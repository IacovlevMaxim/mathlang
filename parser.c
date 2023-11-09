#include "parser.h"
#include "stack.h"
#include "tokens.h"

#include <stdio.h>
#include <stdlib.h>

sstack_t *parse_op(sstack_t *stack, int debug) {
  int op_deapth = 1;
  sstack_t *op_line = init_stack();
  node_t *n;
  // TODO expand logic handle:
  // [x] one-argument oparations such as `not`, `input`, etc;
  //    if, while, and keywords alike;
  // [] maybe type checking certain operations;
  // [] returning error if operation stack is not fully consumed.

  while (op_deapth != 0 && (n = pop_node(stack)) && n->next != NULL) {
    if (n->tok_class == COND_OP || n->tok_class == PUNCT) {
      op_deapth--;
      if (debug)
        printf("(-)%s, d=%i\n", n->str, op_deapth);
    } else if (n->tok_type == NOT) {
      if (debug)
        printf("( )%s, d=%i\n", n->str, op_deapth);
    } else if (n->tok_class == OPERATION) {
      op_deapth++;
      if (debug)
        printf("(+)%s, d=%i\n", n->str, op_deapth);
    } else if (n->tok_class == VALUE || n->tok_class == ID) {
      op_deapth--;
      if (debug)
        printf("(-)%s, d=%i\n", n->str, op_deapth);
    } else {
      if (debug)
        printf("unhandled token: %s\n", n->str);
    }
    push_node(op_line, n);
  }
  if (op_deapth != 0) {
    printf("Parser Error: parse_op failed, op_deapth == %i != 0, node: %s\n",
           op_deapth, n->str);
    return NULL;
  }
  node_t *m = op_line->node;
  if (debug) {
    printf("operation parsed:\n");
    while (m != NULL) {
      printf("%s ", m->str);
      m = m->next;
    }
    printf("\n");
  }
  return op_line;
}

sstack_t *parse_main(sstack_t *stack, int debug) {
  sstack_t *block = init_stack();
  sstack_t *op_line = NULL;
  while (stack->node->next != NULL) {
    if (debug)
      printf("pb: %s\n", stack->node->str);
    if (stack->node->tok_class == VALUE || stack->node->tok_class == ID) {
      fprintf(stderr,
              "Parser Error: parse_block failed. Ilegal placement of '%s' at "
              "the beginning of a line\n",
              stack->node->str);
      return NULL;
    }
    op_line = parse_op(stack, debug);
    if (op_line == NULL) {
      fprintf(stderr, "Parser Error: parse_block -> parse_op failed.\n");
      return NULL;
    }
    if (join_stacks(block, op_line, debug) == 0) {
      fprintf(stderr, "Parser Error: parse_block -> join_stacks failed.\n");
      return NULL;
    }
  }
  return block;
}

sstack_t *parse_all(sstack_t *stack, int debug) {
  node_t *n;
  sstack_t *caveman_tree = init_stack();

  while ((n = pop_node(stack)) && n->next->tok_class != PUNCT && n != NULL) {
    // printf("%s\n", n->str);
    free(n);
  }
  n = pop_node(stack);
  // printf("%s\n", n->str);
  if (n->tok_type == R_BRACE) {
    fprintf(stderr,
            "Parser Error: parse_all failed. Illegal placement of: '%s'\n",
            n->str);
    return NULL;
  }
  return parse_main(stack, debug);
  // return NULL;
}
