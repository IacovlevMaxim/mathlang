#include "parser.h"
#include "stack.h"
#include "stdio.h"
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
  // TODO check again if logic actually makes sense
  for (; op_deapth != 1 && n->tok_class != PUNCT && n->next != NULL;
       n = pop_node(stack)) {
    // TODO expand logic handle:
    // one-argument oparations such as `not`, `input`, etc;
    // if, while, and keywords alike;
    // maybe type checking certain operations;
    // returning error if operation stack is not fully consumed.
    if (n->tok_class == OPERATION)
      op_deapth++;
    else if (n->tok_class == VALUE || n->tok_class == ID)
      op_deapth--;
    push_node(op_line, n);
  }
  if (op_deapth != 0)
    return NULL;
  return op_line;
}

sstack_t *parse_block(sstack_t *stack) {
  if (stack->node->tok_type != L_BRACE)
    return NULL;
  free(pop_node(stack));

  sstack_t *block = init_stack();
  // TODO handle braces inside main block
  while (stack->node->tok_type != R_BRACE) {
    if (stack->node->tok_class != OPERATION)
      return NULL;
    sstack_t *op_line = parse_op(stack);
    if (op_line == NULL)
      return NULL;
    if (join_stacks(block, op_line) == 0)
      return NULL;
  }
  return block;
}

sstack_t *parse_all(sstack_t *stack) {
  node_t *n;
  sstack_t *caveman_tree = init_stack();
  for (n = pop_node(stack); n->tok_class != PUNCT && n->next != NULL;
       n = pop_node(stack)) {
    free(n);
  }
  if (n->tok_type != L_BRACE) {
    return 0;
  }
  return parse_block(stack);
}
