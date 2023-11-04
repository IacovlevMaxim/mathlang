#include "lexer.h"
#include "parser.h"
#include "stdio.h"
#include <stdlib.h>

int main() {
  stack_t *stack = malloc(sizeof(stack_t));
  stack->node = NULL;
  stack_t *def = malloc(sizeof(stack_t));
  def->node = NULL;
  node_t *tokens[10];

  for (int i = 0; i < 10; i++) {
    tokens[i] = init_node();
  }

  tokens[0]->tok_class = DEF_OP;
  tokens[0]->tok_type = DEF_INT;
  tokens[0]->str = "int";

  tokens[1]->tok_class = ID;
  tokens[1]->tok_type = INT;
  tokens[1]->str = "a";

  tokens[2]->tok_class = ID;
  tokens[2]->tok_type = INT;
  tokens[2]->str = "b";

  tokens[3]->tok_class = PUNCT;
  tokens[3]->tok_type = L_BRACE;
  tokens[3]->str = "{";

  tokens[4]->tok_class = OPERATION;
  tokens[4]->tok_type = ASG;
  tokens[4]->str = "asg";

  tokens[5]->tok_class = ID;
  tokens[5]->tok_type = INT;
  tokens[5]->str = "num";

  tokens[6]->tok_class = OPERATION;
  tokens[6]->tok_type = ADD;
  tokens[6]->str = "add";

  tokens[7]->tok_class = ID;
  tokens[7]->tok_type = INT;
  tokens[7]->str = "a";

  tokens[8]->tok_class = ID;
  tokens[8]->tok_type = INT;
  tokens[8]->str = "b";

  tokens[9]->tok_class = PUNCT;
  tokens[9]->tok_type = R_BRACE;
  tokens[9]->str = "}";

  parse(stack, def, tokens, 10);

  node_t *n;

  printf("def block:\n");
  while (NULL != (n = pop_node(def))) {
    printf("\tdef->str: %s\n", n->str);
  }

  printf("exec block:\n");
  while (NULL != (n = pop_node(stack))) {
    printf("\texe->str: %s\n", n->str);
  }
  return EXIT_SUCCESS;
}
