#include "lexer.h"
#include "parser.h"
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
  parse_code(code, stack, 0);

  sstack_t *caveman_tree = parse_all(stack);
  if (caveman_tree == NULL) {
    fprintf(stderr, "Test Error: parser failed.\n");
    exit(1);
  }

  node_t *curr = pop_node(caveman_tree);
  while (curr != NULL) {
    printf("%s ", curr->str);
    curr = pop_node(caveman_tree);
  }
}
