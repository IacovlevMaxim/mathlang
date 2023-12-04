#include "interpreter.h"
#include "lexer.h"
#include "parser.h"
#include "stack.h"
#include "stdio.h"
#include <stdlib.h>
#include <string.h>

// const int SIZE = 20;

void input_code(char **code) {
  printf("Input code:\n");
  int main_code = 0, depth = 0;
  char *line = strdup("");

  while (1) {
    //        scanf("%s\n", line);
    fgets(line, 64, stdin);
    for (int i = 0; line[i] != '\0'; i++) {
      if (line[i] == '{') {
        if (main_code == 0)
          main_code = 1;
        depth++;
      } else if (line[i] == '}') {
        depth--;
      }
    }

    strcat(*code, line);
    if (main_code == 1 && depth == 0)
      return;
  }
}

int main() {
  char *code = malloc(sizeof(char) * 256);
  input_code(&code);
  // char *code = "int a\n"
  //              "float b\n"
  //              "{\n"
  //              "  { }\n"
  //              "}\n";

  sstack_t *stack = init_stack();

  // After using variables, use free(variables);
  var *variables = malloc(sizeof(var) * MAX_VAR_AMOUNT);
  tokenize(code, stack, &variables, 0);

  // After using nodes, use free(node) for each node
  sstack_t *caveman_tree = parse_tokens(stack, 0);
  if (caveman_tree == NULL) {
    fprintf(stderr, "Test Error: parser failed.\n");
    exit(1);
  }

  interpret(caveman_tree, variables, 0);

  node_t *n;
  while ((n = pop_node(caveman_tree)) != NULL)
    free(n);
}
