#include <regex.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

#define MAX_VAR_AMOUNT 10


//Maybe without the caret?
const char* IS_VAR = "^[a-zA-Z_][a-zA-Z0-9]*";

node_t EMPTY_DATA = {
        CLASS_NONE,
        TYPE_NONE,
        ""
};

regex_t var_regex;

int is_integer(char* str) {
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

int is_float(char* str) {
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

int get_op_type(char* str) {
    if(strcmp(str, "asg") == 0) return ASG;
    if(strcmp(str, "add") == 0) return ADD;
    if(strcmp(str, "sub") == 0) return SUB;
    if(strcmp(str, "div") == 0) return DIV;
    if(strcmp(str, "mul") == 0) return MUL;
    if(strcmp(str, "eq") == 0)  return EQ;
    if(strcmp(str, "gt") == 0)  return GT;
    if(strcmp(str, "lt") == 0)  return LT;
    if(strcmp(str, "not") == 0) return NOT;
    if(strcmp(str, "and") == 0) return AND;
    if(strcmp(str, "or") == 0)  return OR;
    return TYPE_NONE;
}

int get_cond_op_type(char* str) {
    if(strcmp(str, "while") == 0) return WHILE;
    if(strcmp(str, "if") == 0)    return IF;
    if(strcmp(str, "else") == 0)  return ELSE;
    return TYPE_NONE;
}

void parse_code(char* code, node_t* top) {
    if (regcomp(&var_regex, IS_VAR, REG_EXTENDED)) {
        fprintf(stderr, "Lexer error: Failed to compile var_regex");
        exit(1);
    }

    var vars[MAX_VAR_AMOUNT];
    int curr_type = TYPE_NONE;
    int var_amount = 0;
    int line_count = 0;

    char* token = malloc(sizeof(char) * strlen(code));

    for(int i = 0;i < strlen(code);i++) {
        char c = *(code + i);
//        printf("looking at '%c'\n", c);
        if (c != ' ' && c != '\n') {
//            printf("skipping '%c'\n", c);
            strncat(token, &c, 1);
            continue;
        }

        if(c == '\n') line_count++;

//        printf("checking '%s'\n", token);
        node_t *curr;
        curr = malloc(sizeof(node_t));
        if(curr == NULL) {
            fprintf(stderr, "Lexer error: Failed to allocate memory for node");
            exit(1);
        }
        curr->next = NULL;
        int op_type = get_op_type(token);
        int cond_op_type = get_cond_op_type(token);

        if (strcmp(token, "int") == 0) {
            curr_type = INT;
            curr->tok_class = DEF_OP;
            curr->tok_type = DEF_INT;
            curr->str = strdup(token);
        } else if (strcmp(token, "float") == 0) {
            curr_type = FLOAT;
            curr->tok_class = DEF_OP;
            curr->tok_type = DEF_FLOAT;
            curr->str = strdup(token);
        } else if(strcmp(token, "{") == 0) {
            curr->tok_class = PUNCT;
            curr->tok_type = L_BRACE;
            curr->str = "{";
        } else if(strcmp(token, "}") == 0) {
            curr->tok_class = PUNCT;
            curr->tok_type = R_BRACE;
            curr->str = "}";
        } else if (op_type != TYPE_NONE) {
            curr->tok_class = OPERATION;
            curr->tok_type = op_type;
            curr->str = strdup(token);
        } else if (cond_op_type != TYPE_NONE) {
            curr->tok_class = COND_OP;
            curr->tok_type = cond_op_type;
            curr->str = strdup(token);
        } else if(is_integer(strdup(token)) == 1) {
            curr->tok_class = VALUE;
            curr->tok_type = INT;
            curr->str = strdup(token);
        } else if(is_float(strdup(token)) == 1) {
            curr->tok_class = VALUE;
            curr->tok_type = FLOAT;
            curr->str = strdup(token);
        } else if (!regexec(&var_regex, token, 0, NULL, 0)) {
            int exists = 0;
            for (int j = 0; j < var_amount; j++) {
                if (strcmp(vars[j].name, token) == 0) {
                    exists = 1;
                    break;
                }
            }

            if (exists == 0) {
                if(var_amount == MAX_VAR_AMOUNT) {
                    fprintf(stderr, "Lexer error: Reached max amount of variables (%d)", MAX_VAR_AMOUNT);
                    exit(1);
                }
                var new_var = {
                        strdup(token),
                        curr_type
                };
                vars[var_amount] = new_var;
                var_amount++;
            } else if(curr_type != TYPE_NONE) {
                fprintf(stderr, "Lexer error: Redefining variable '%s' on line %d", token, line_count);
                exit(1);
            }

            curr->tok_class = ID;
            curr->tok_type = curr_type;
            curr->str = strdup(token);

//            printf("next char after %s is: '%c'", token, *(code + i));
            if (*(code + i) == '\n') {
                curr_type = TYPE_NONE;
            }
        } else {
            if(strcmp(token, "") == 0) {
                fprintf(stderr, "Lexer error: Empty line %d", line_count);
            } else {
                fprintf(stderr, "Lexer error: Unexpected token '%s' on line %d", token, line_count);
            }
            exit(1);
        }

        token[0] = '\0';
        append_node(top, curr);
    }
}

int main(void) {
    char* code =
            "int a\n" \
            "float b\n" \
            "{\n" \
            "asg a 1\n" \
            "asg b 1.23\n" \
            "while not eq a 10 {\n" \
            "asg a add a 1\n" \
            "}\n" \
            "}\n";

    printf("Code:\n///\n%s\n///\n", code);
    node_t node = EMPTY_DATA;
    parse_code(code, &node);
    printf("Node contents:\n");
    printf("str: %s\n", node.str);
    printf("type: %d\n", node.tok_type);
    printf("class: %d\n", node.tok_class);
    while(node.next != NULL) {
        printf("-----\n");
        printf("str: %s\n", node.next->str);
        printf("type: %d\n", node.next->tok_type);
        printf("class: %d\n", node.next->tok_class);
        node = *node.next;
    }

    //To-do: also deallocate memory xd
}