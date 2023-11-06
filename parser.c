#include "parser.h"
#include "lexer.h"
#include "stdio.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

node_t *init_node() {
  node_t *node;
  node = malloc(sizeof(node_t));
  if (node == NULL)
    return NULL;
  // node->data = alloc_data_of(type);
  node->next = NULL;
  return node;
}

int push_node(sstack_t *stack, node_t *to_push) {
  if (to_push == NULL || stack == NULL)
    return 0;
  // printf("%s\n", to_push->str);
  to_push->next = stack->node;
  stack->node = to_push;
  return 1;
}

node_t *pop_node(sstack_t *stack) {
  node_t *to_pop = stack->node;
  if (stack->node == NULL)
    return NULL;
  stack->node = stack->node->next;
  // printf("%s\n", to_pop->str);
  return to_pop;
}

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

// void parse_exec(stack_t *tree[], stack_t *stack) {}

// int register_vars(registr_t *registr, char *var, token_type_t type) {
//   // int exists = 0;
//   int i = 0;
//   for (; i < registr->pos; i++) {
//     if (registr->names[i] == var) {
//       return i;
//       break;
//     }
//   }
//   registr->pos++;
//
//   node_val_t *temp_v =
//       realloc(registr->vals, sizeof(node_val_t) * registr->size * 1.5);
//   if (temp_v == NULL) {
//     free(temp_v);
//     return -1;
//   }
//   registr->vals = temp_v;
//
//   char **temp_n = realloc(registr->vals, sizeof(char *) * registr->size);
//   if (temp_v == NULL) {
//     free(temp_v);
//     return -1;
//   }
//   registr->vals = temp_v;
//
//   registr->names[i] = var;
//   return i;
// }
