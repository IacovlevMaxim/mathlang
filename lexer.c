#include "lexer.h"
#include "stack.h"
#include "tokens.h"
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Maybe without the caret?
const char *IS_VAR = "^[a-zA-Z][a-zA-Z0-9_]*";

regex_t var_regex;

int is_integer(char *str) {
  if (str == NULL || *str == '\0') {
    return 0;
  }

  if (*str == '+' || *str == '-') {
    str++;
  }

  while (*str != '\0') {
    if (*str < '0' || *str > '9') {
      return 0;
    }
    str++;
  }

  return 1;
}

int is_float(char *str) {
  if (str == NULL || *str == '\0') {
    return 0;
  }

  if (*str == '+' || *str == '-') {
    str++;
  }

  int decimalPointSeen = 0;

  while (*str != '\0') {
    if (*str == '.') {
      if (decimalPointSeen == 1) {
        return 0;
      }
      decimalPointSeen = 1;
    } else if (*str < '0' || *str > '9') {
      return 0;
    }
    str++;
  }

  return decimalPointSeen;
}

int get_op_type(char *str) {
  if (strcmp(str, "asg") == 0)
    return ASG;
  if (strcmp(str, "add") == 0)
    return ADD;
  if (strcmp(str, "sub") == 0)
    return SUB;
  if (strcmp(str, "div") == 0)
    return DIV;
  if (strcmp(str, "mul") == 0)
    return MUL;
  if (strcmp(str, "eq") == 0)
    return EQ;
  if (strcmp(str, "gt") == 0)
    return GT;
  if (strcmp(str, "lt") == 0)
    return LT;
  if (strcmp(str, "not") == 0)
    return NOT;
  if (strcmp(str, "and") == 0)
    return AND;
  if (strcmp(str, "or") == 0)
    return OR;
  if (strcmp(str, "print") == 0)
      return PRINT;
  if (strcmp(str, "input") == 0)
      return INPUT;
  return TYPE_NONE;
}

int get_cond_op_type(char *str) {
  if (strcmp(str, "while") == 0)
    return WHILE;
  if (strcmp(str, "if") == 0)
    return IF;
  if (strcmp(str, "else") == 0)
    return ELSE;
  return TYPE_NONE;
}

void tokenize(char *code, sstack_t *top, var **variables, int debug) {
  if (regcomp(&var_regex, IS_VAR, REG_EXTENDED)) {
    fprintf(stderr, "Lexer error: Failed to compile var_regex");
    exit(1);
  }

  //  var vars[MAX_VAR_AMOUNT];
  int curr_type = TYPE_NONE;
  int var_amount = 0;
  int line_count = 1;
  int depth = 0;
  int skip_append = 0;

  // char *token = malloc(sizeof(char) * strlen(code));
  char *token = strdup("");

  for (int i = 0; i < strlen(code); i++) {
    char c = *(code + i);
    if (debug)
      printf("looking at '%c' (%d)\n", c, c);
    if (c != ' ' && c != '\n' && c != '\t') {
      if (debug)
        printf("skipping '%c'\n", c);
      strncat(token, &c, 1);
      continue;
    }

    if (strlen(token) == 0) {
      if (debug)
        printf("skipping '%s'", token);
      continue;
    }

    if (c == '\n')
      line_count++;

    if (debug)
      printf("checking '%s'\n", token);

    node_t *curr;
    curr = init_node();
    if (curr == NULL) {
      fprintf(stderr, "Lexer error: Failed to allocate memory for node");
      exit(1);
    }
    curr->next = NULL;
    int op_type = get_op_type(token);
    int cond_op_type = get_cond_op_type(token);

    if (strcmp(token, "int") == 0) {
      if (depth > 0) {
        fprintf(stderr,
                "Lexer error: Definition of variable in main block on line %d",
                line_count);
        exit(1);
      }
      curr_type = INT;
      skip_append = 1;
      //      curr->tok_class = DEF_OP;
      //      curr->tok_type = DEF_INT;
      //      curr->str = strdup(token);
    } else if (strcmp(token, "float") == 0) {
      if (depth > 0) {
        fprintf(stderr,
                "Lexer error: Definition of variable in main block on line %d",
                line_count);
        exit(1);
      }
      curr_type = FLOAT;
      skip_append = 1;
      //      curr->tok_class = DEF_OP;
      //      curr->tok_type = DEF_FLOAT;
      //      curr->str = strdup(token);
    } else if (strcmp(token, "{") == 0) {
      depth++;
      if (depth != 1) {
        curr->tok_class = PUNCT;
        curr->tok_type = L_BRACE;
        curr->str = "{";
      } else {
        skip_append = 1;
      }
    } else if (strcmp(token, "}") == 0) {
      depth--;
      if (depth < 0) {
        fprintf(stderr, "Lexer error: Extra right bracket on line %d",
                line_count);
        exit(1);
      }
      if (depth > 0) {
        curr->tok_class = PUNCT;
        curr->tok_type = R_BRACE;
        curr->str = "}";
      } else {
        skip_append = 1;
      }
    } else if (op_type != TYPE_NONE) {
      curr->tok_class = OPERATION;
      curr->tok_type = op_type;
      curr->str = strdup(token);
    } else if (cond_op_type != TYPE_NONE) {
      curr->tok_class = COND_OP;
      curr->tok_type = cond_op_type;
      curr->str = strdup(token);
    } else if (is_integer(strdup(token)) == 1) {
      curr->tok_class = VALUE;
      curr->tok_type = INT;
      curr->str = strdup(token);
    } else if (is_float(strdup(token)) == 1) {
      curr->tok_class = VALUE;
      curr->tok_type = FLOAT;
      curr->str = strdup(token);
    } else if (!regexec(&var_regex, token, 0, NULL, 0)) {
      if (depth == 0)
        skip_append = 1;
      int var_type = TYPE_NONE;

      for (int j = 0; j < var_amount; j++) {
        if (strcmp((*variables + j)->name, token) == 0) {
          var_type = (*variables + j)->type;
          break;
        }
      }

      if (var_type == TYPE_NONE) {
        if (var_amount == MAX_VAR_AMOUNT) {
          fprintf(stderr, "Lexer error: Reached max amount of variables (%d)",
                  MAX_VAR_AMOUNT);
          exit(1);
        }
        var new_var = {strdup(token), curr_type};
        if (curr_type == INT) {
          new_var.value.i = 0;
        } else if (curr_type == FLOAT) {
          new_var.value.f = 0;
        } else {
          fprintf(
              stderr,
              "Lexer error: No type definition for variable '%s' on line %d",
              token, line_count);
          exit(1);
        }
        //        vars[var_amount] = new_var;
        *(*variables + var_amount) = new_var;
        var_amount++;
      } else if (curr_type != TYPE_NONE) {
        fprintf(stderr, "Lexer error: Redefining variable '%s' on line %d",
                token, line_count);
        exit(1);
      }

      curr->tok_class = ID;
      if (var_type != TYPE_NONE) {
        curr->tok_type = var_type;
      } else {
        curr->tok_type = curr_type;
      }

      if (curr->tok_type == 0) {
        fprintf(stderr, "Lexer error: No type definition for variable '%s'",
                token);
        exit(1);
      }

      curr->str = strdup(token);

      if (debug)
        printf("next char after %s is: '%c'\n", token, *(code + i));
      if (*(code + i) == '\n') {
        curr_type = TYPE_NONE;
      }
    } else {
      //      if (strcmp(token, "") != 0) {
      fprintf(stderr, "Lexer error: Unexpected token '%s' on line %d", token,
              line_count);

      //      }
      exit(1);
    }

    // token[0] = '\0';
    token = strdup("");
    if (debug)
      printf("to append: '%s'\n", curr->str);
    if (skip_append == 1) {
      skip_append = 0;
      continue;
    }
    if (append_node(top, curr) == 0) {
      fprintf(stderr, "Lexer error: failed to append token '%s'\n", curr->str);
      exit(1);
    }
  }

  if (depth != 0) {
    fprintf(stderr, "Lexer error: Not all code blocks are enclosed");
    exit(1);
  }

  free(token);
  //  *variables = vars;
}

// int main(void) {
//     char* code =
//             "int a\n" \
//            "float b\n" \
//            "{\n" \
//            "asg a 1\n" \
//            "asg b 1.23\n" \
//            "while not eq a 10 {\n" \
//            "asg a add a 1\n" \
//            "}\n" \
//            "}\n";
//
//     printf("Code:\n///\n%s\n///\n", code);
//     node_t node = EMPTY_DATA;
//     parse_code(code, &node);
//     printf("Node contents:\n");
//     printf("str: %s\n", node.str);
//     printf("type: %d\n", node.tok_type);
//     printf("class: %d\n", node.tok_class);
//     while(node.next != NULL) {
//         printf("-----\n");
//         printf("str: %s\n", node.next->str);
//         printf("type: %d\n", node.next->tok_type);
//         printf("class: %d\n", node.next->tok_class);
//         node = *node.next;
//     }
//
//     //To-do: also deallocate memory xd
// }
