#include "parser.h"
#include "stack.h"
#include "tokens.h"

#include <stdio.h>
#include <stdlib.h>

// Old func as refference:

// void parse(sstack_t *exec_stack, node_t *tokens[], size_t size) {
//   int i = 0;
//   int op_deapth = 1;
//   int op_line = 0;
//
//   if (tokens[i]->tok_type != L_BRACE) {
//     printf("illegal placement of %s\n", tokens[i]->str);
//     exit(EXIT_FAILURE);
//   }
//   int offset = i + 1;
//   for (++i; tokens[i]->tok_class != PUNCT && i < size; i++) {
//     if (tokens[i]->tok_class == OPERATION)
//       op_deapth++;
//     else if (tokens[i]->tok_class == VALUE || tokens[i]->tok_class == ID)
//       op_deapth--;
//     push_node(exec_stack, tokens[i]);
//     if (op_deapth == 0) {
//       printf("line %i ends at token %i\n", op_line, i - offset);
//       op_deapth++;
//       op_line++;
//     }
//   }
// }

sstack_t *parse_op(sstack_t *stack) {
  int op_deapth = 1;
  sstack_t *op_line = init_stack();
  node_t *n;
  // TODO expand logic handle:
  // one-argument oparations such as `not`, `input`, etc;
  // if, while, and keywords alike;
  // maybe type checking certain operations;
  // returning error if operation stack is not fully consumed.

  while (op_deapth != 0 && (n = pop_node(stack)) && n->tok_class != PUNCT &&
         n->next != NULL) {
    if (n->tok_class == OPERATION) {
      op_deapth++;
      printf("+%s, d=%i\n", n->str, op_deapth);
    } else if (n->tok_class == VALUE || n->tok_class == ID) {
      op_deapth--;
      printf("-%s, d=%i\n", n->str, op_deapth);
    }
    push_node(op_line, n);
  }
  if (op_deapth != 0) {
    printf("Parser Error: parse_op failed, op_deapth == %i != 0\n", op_deapth);
    return NULL;
  }
  node_t *m = op_line->node;
  printf("operation parsed:\n");
  while (m != NULL) {
    printf("%s ", m->str);
    m = m->next;
  }
  printf("\n");
  // printf("%s, %s\n", n->str, stack->node->str);
  return op_line;
}

sstack_t *parse_block(sstack_t *stack) {
  sstack_t *block = init_stack();
  sstack_t *op_line = NULL;
  // TODO handle braces inside main block
  while (stack != NULL && stack->node != NULL &&
         stack->node->tok_type != R_BRACE) {
    printf("pb: %s\n", stack->node->str);
    if (stack->node->tok_class != OPERATION) {
      fprintf(stderr,
              "Parser Error: parse_block failed. Ilegal placement of '%s' at "
              "the beginning of a line\n",
              stack->node->str);
      return NULL;
    }
    op_line = parse_op(stack);
    if (op_line == NULL) {
      fprintf(stderr, "Parser Error: parse_block -> parse_op failed.\n");
      return NULL;
    }
    // if (join_stacks(block, op_line) == 0) {
    //   fprintf(stderr, "Parser Error: parse_block -> join_stacks failed.\n");
    //   return NULL;
    // }
  }
  return block;
}

sstack_t *parse_all(sstack_t *stack) {
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
  return parse_block(stack);
  // return NULL;
}
