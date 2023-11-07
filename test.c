#include "lexer.h"
// #include "parser.h"
#include "stack.h"
#include "stdio.h"
#include <stdlib.h>

// const int SIZE = 20;

int main() {
  char *code = "int a\n"
               "float b\n"
               "{\n"
               "asg a 1\n"
               "asg b 1.23\n"
               "while not eq a 10 {\n"
               "asg a add a 1\n"
               "}\n"
               "}\n";

  printf("Code:\n///\n%s\n///\n", code);
  //  node_t node = EMPTY_DATA;
  sstack_t *stack = init_stack();
  parse_code(code, stack);

  node_t *curr = pop_node(stack);
  while (curr != NULL) {
    printf("%s", curr->str);
    curr = pop_node(stack);
  }

  //  printf("Node contents:\n");
  //  printf("str: %s\n", node.str);
  //  printf("type: %d\n", node.tok_type);
  //  printf("class: %d\n", node.tok_class);
  //  while (node.next != NULL) {
  //    printf("-----\n");
  //    printf("str: %s\n", node.next->str);
  //    printf("type: %d\n", node.next->tok_type);
  //    printf("class: %d\n", node.next->tok_class);
  //    node = *node.next;
  //  }
}

// int main() {
//   sstack_t *stack = malloc(sizeof(sstack_t));
//   stack->node = NULL;
//   sstack_t *def = malloc(sizeof(sstack_t));
//   def->node = NULL;
//   node_t *tokens[SIZE];
//
//   for (int i = 0; i < SIZE; i++) {
//     tokens[i] = init_node();
//   }
//
//   tokens[0]->tok_class = DEF_OP;
//   tokens[0]->tok_type = DEF_INT;
//   tokens[0]->str = "int";
//
//   tokens[1]->tok_class = ID;
//   tokens[1]->tok_type = INT;
//   tokens[1]->str = "a";
//
//   tokens[2]->tok_class = ID;
//   tokens[2]->tok_type = INT;
//   tokens[2]->str = "b";
//
//   tokens[3]->tok_class = ID;
//   tokens[3]->tok_type = INT;
//   tokens[3]->str = "num";
//
//   tokens[4]->tok_class = PUNCT;
//   tokens[4]->tok_type = L_BRACE;
//   tokens[4]->str = "{";
//
//   tokens[5]->tok_class = OPERATION;
//   tokens[5]->tok_type = ASG;
//   tokens[5]->str = "asg";
//
//   tokens[6]->tok_class = ID;
//   tokens[6]->tok_type = INT;
//   tokens[6]->str = "num";
//
//   tokens[7]->tok_class = OPERATION;
//   tokens[7]->tok_type = ADD;
//   tokens[7]->str = "add";
//
//   tokens[8]->tok_class = ID;
//   tokens[8]->tok_type = INT;
//   tokens[8]->str = "a";
//
//   tokens[9]->tok_class = ID;
//   tokens[9]->tok_type = INT;
//   tokens[9]->str = "b";
//
//   tokens[10]->tok_class = OPERATION;
//   tokens[10]->tok_type = ASG;
//   tokens[10]->str = "asg";
//
//   tokens[11]->tok_class = ID;
//   tokens[11]->tok_type = INT;
//   tokens[11]->str = "b";
//
//   tokens[12]->tok_class = OPERATION;
//   tokens[12]->tok_type = ADD;
//   tokens[12]->str = "add";
//
//   tokens[13]->tok_class = OPERATION;
//   tokens[13]->tok_type = MUL;
//   tokens[13]->str = "mul";
//
//   tokens[14]->tok_class = ID;
//   tokens[14]->tok_type = INT;
//   tokens[14]->str = "a";
//
//   tokens[15]->tok_class = VALUE;
//   tokens[15]->tok_type = INT;
//   tokens[15]->str = "69";
//
//   tokens[16]->tok_class = OPERATION;
//   tokens[16]->tok_type = DIV;
//   tokens[16]->str = "div";
//
//   tokens[17]->tok_class = ID;
//   tokens[17]->tok_type = INT;
//   tokens[17]->str = "b";
//
//   tokens[18]->tok_class = VALUE;
//   tokens[18]->tok_type = INT;
//   tokens[18]->str = "42";
//
//   tokens[19]->tok_class = PUNCT;
//   tokens[19]->tok_type = R_BRACE;
//   tokens[19]->str = "}";
//
//   parse(stack, def, tokens, SIZE);
//
//   node_t *n;
//
//   printf("def block:\n");
//   while (NULL != (n = pop_node(def))) {
//     printf("\tdef->str: %s\n", n->str);
//   }
//
//   printf("exec block:\n");
//   while (NULL != (n = pop_node(stack))) {
//     printf("\texe->str: %s\n", n->str);
//   }

// TODO free memory xdd

// return EXIT_SUCCESS;
// }
