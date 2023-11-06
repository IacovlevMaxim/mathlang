#include "stdio.h"
#include <stdlib.h>
#include "parser.h"
#include "stack.h"

void parse(sstack_t *exec_stack, sstack_t *def_stack, node_t *tokens[],
           size_t size) {
  int i = 0;
  int op_deapth = 1;
  int op_line = 0;

  // exec_stack = malloc(sizeof(stack_t));

  for (; tokens[i]->tok_class != PUNCT && i < size; i++)
    push_node(def_stack, tokens[i]);
  // printf("tok->%s\n", tokens[i]->str);
  if (tokens[i]->tok_type != L_BRACE) {
    printf("illegal placement of %s\n", tokens[i]->str);
    exit(EXIT_FAILURE);
  }
  int offset = i + 1;
  for (++i; tokens[i]->tok_class != PUNCT && i < size; i++) {
    if (tokens[i]->tok_class == OPERATION)
      op_deapth++;
    else if (tokens[i]->tok_class == VALUE || tokens[i]->tok_class == ID)
      op_deapth--;
    push_node(exec_stack, tokens[i]);
    if (op_deapth == 0) {
      printf("line %i ends at token %i\n", op_line, i - offset);
      op_deapth++;
      op_line++;
    }
  }
}