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
               "\n\n"
               "    asg a 1\n"
               "    asg b 1.23\n"
               "    if gt b 1 {\n"
               "        while not eq a 10 {\n"
               "            asg a add a 1\n"
               "        }\n"
               "    }\n"
               "}\n";

  printf("Code:\n///\n%s///\n", code);
  //   node_t node = EMPTY_DATA;
  sstack_t *stack = init_stack();

  //After using variables, use free(variables);
  var *variables = malloc(sizeof(var) * MAX_VAR_AMOUNT);
  tokenize(code, stack, &variables, 1);

    node_t *stack_curr = stack->node;

    while(stack_curr != NULL) {
        printf("class: %d\n", stack_curr->tok_class);
        printf("type: %d\n", stack_curr->tok_type);
        printf("str: %s\n", stack_curr->str);
        printf("------\n");

        stack_curr = stack_curr->next;
//      token_class_t tok_class;
//      token_type_t tok_type;
//      char *str;
//      // size_t no_line;
//      node_val_t *val;
    }


  int i = 0;
  printf("Variables:\n");
  var* currVar;
  while((currVar = (variables + i))->type) {
      printf("%s: %d with val ", currVar->name, currVar->type);
      if(currVar->type == INT) {
          printf("%d\n", currVar->value.i);
      } else if(currVar->type == FLOAT) {
          printf("%f\n", currVar->value.f);
      } else {
          printf("No type for variable!");
          exit(1);
      }
      i++;
  }

  //After using nodes, use free(node) for each node
  sstack_t *caveman_tree = parse_all(stack, 0);
  if (caveman_tree == NULL) {
    fprintf(stderr, "Test Error: parser failed.\n");
    exit(1);
  }
  printf("Parsed:\n");
  node_t *curr = pop_node(caveman_tree);
  while (curr != NULL) {
    printf("%s ", curr->str);
    curr = pop_node(caveman_tree);
  }
  printf("\n");
}
