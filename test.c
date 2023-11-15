#include "lexer.h"
#include "parser.h"
#include "interpreter.h"
#include "stack.h"
#include "stdio.h"
#include <stdlib.h>
#include <string.h>

// const int SIZE = 20;

void input_code(char** code) {
    printf("Input code:\n");
    int main_code = 0, depth = 0;
    char *line = strdup("");

    while(1) {
//        scanf("%s\n", line);
        fgets(line, 64, stdin);
        for(int i = 0; line[i] != '\0';i++) {
            if(line[i] == '{') {
                if(main_code == 0) main_code = 1;
                depth++;
            } else if(line[i] == '}') {
                depth--;
            }
        }

        strcat(*code, line);
        if(main_code == 1 && depth == 0) return;
    }
}

//int main() {
//    printf("Started main\n");
//    var a_var = {
//           "a",
//           1,
//           {
//                -1
//           }
//    };
//    var* variables = malloc(sizeof(var) * 4);
//    variables[0] = a_var;
//    sstack_t* stack = init_stack();
//
//    node_t* one = init_node();
//    one->tok_class = VALUE;
//    one->tok_type = INT;
//    one->str = "1";
//    one->val->i = 1;
//
//    node_t* a = init_node();
//    a->tok_class = ID;
//    a->tok_type = FLOAT;
//    a->str = "a";
//    a->val->f = a_var.value.f;
//
//
////    Testing asg
//    node_t *asg = init_node();
//    asg->tok_class = OPERATION;
//    asg->tok_type = ASG;
//    asg->str = "asg";
//
////  Testing add
////    node_t *add = init_node();
////    add->tok_class = OPERATION;
////    add->tok_type = ADD;
////    add->str = "add";
//
//    //Update next here
//    a->next = asg;
//    one->next = a;
//    stack->node = one;
//
//    printf("Finished generating stuff\n");
//    interpret(stack, &variables, 1);
//
//    int i = 0;
//    printf("Variables:\n");
//    var* currVar;
//    while((currVar = (variables + i))->type) {
//        printf("%s: %d with val ", currVar->name, currVar->type);
//        if(currVar->type == INT) {
//            printf("%d\n", currVar->value.i);
//        } else if(currVar->type == FLOAT) {
//            printf("%f\n", currVar->value.f);
//        } else {
//            printf("No type for variable!");
//            exit(1);
//        }
//        i++;
//    }
//    return 0;
//}

int main() {
//    char *code = malloc(sizeof(char) * 256);
//    input_code(&code);
  char *code = "int a\n"
               "float b\n"
               "{\n"
               "\n\n"
               "    asg a 1\n"
               "    asg b 1.23\n"
//               "    asg a add a 2\n"
               "}\n";

  printf("Code:\n///\n%s///\n", code);
  //   node_t node = EMPTY_DATA;
  sstack_t *stack = init_stack();

  //After using variables, use free(variables);
  var *variables = malloc(sizeof(var) * MAX_VAR_AMOUNT);
  tokenize(code, stack, &variables, 1);

//    node_t *stack_curr = stack->node;
//
//    while(stack_curr != NULL) {
//        printf("class: %d\n", stack_curr->tok_class);
//        printf("type: %d\n", stack_curr->tok_type);
//        printf("str: %s\n", stack_curr->str);
//        printf("------\n");
//
//        stack_curr = stack_curr->next;
////      token_class_t tok_class;
////      token_type_t tok_type;
////      char *str;
////      // size_t no_line;
////      node_val_t *val;
//    }


//  int i = 0;
//  printf("Variables:\n");
//  var* currVar;
//  while((currVar = (variables + i))->type) {
//      printf("%s: %d with val ", currVar->name, currVar->type);
//      if(currVar->type == INT) {
//          printf("%d\n", currVar->value.i);
//      } else if(currVar->type == FLOAT) {
//          printf("%f\n", currVar->value.f);
//      } else {
//          printf("No type for variable!");
//          exit(1);
//      }
//      i++;
//  }

  //After using nodes, use free(node) for each node
  sstack_t *caveman_tree = parse_tokens(stack, 0);
  if (caveman_tree == NULL) {
    fprintf(stderr, "Test Error: parser failed.\n");
    exit(1);
  }

  interpret(caveman_tree, &variables, 1);

//  printf("Parsed:\n");
//  node_t *curr = pop_node(caveman_tree);
//  while (curr != NULL) {
//    printf("%s ", curr->str);
//    curr = pop_node(caveman_tree);
//  }
//  printf("\n");
}
