#include "parser.h"
#include "stack.h"
#include "stdio.h"
#include <stdlib.h>

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
  node_t *n;
  for (; n->tok_class != PUNCT && n->next != NULL; n = pop_node(stack)) {
    // TODO parse line
  }
  return NULL;
}

sstack_t *parse_block(sstack_t *stack) {
  if (stack->node->tok_type != L_BRACE)
    return NULL;
  free(pop_node(stack));

  sstack_t *block = init_stack();
  while (stack->node->tok_type != R_BRACE) {
    if (stack->node->tok_class != OPERATION)
      return NULL;
    sstack_t *op_line = parse_op(stack);
    if (op_line == NULL)
      return NULL;
    join_stacks(block, op_line);
  }
  return block;
}

int parse_all(sstack_t *stack) {
  node_t *n;
  for (n = pop_node(stack); n->tok_class != PUNCT && n->next != NULL;
       n = pop_node(stack)) {
    free(n);
  }
  if (n->tok_type != L_BRACE) {
    return 0;
  }
  return parse_block(stack);
}
