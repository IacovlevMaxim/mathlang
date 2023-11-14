#include "stack.h"
#include "lexer.h"
#include "string.h"
#include <stdio.h>
#include <stdlib.h>

var* get_variable(var** variables, char** name) {
    var *currVar;
    int i = 0;
    while((currVar = (*variables + i))->type) {
        if(strcmp(currVar->name, *name) == 0) {
            return currVar;
        }
        i++;
    }

    return NULL;
}

void asg(sstack_t *params, var **variables, int debug) {
    node_t *to = params->node;
    node_t *from = to->next;

    if(to->tok_class != ID) {
        fprintf(stderr, "Interpreter error: assignment target is not a variable: %s\n", to->str);
        exit(1);
    }

    if(to->tok_type == INT && from->tok_type == FLOAT) {
        fprintf(stderr, "Interpreter error: assigning float to int variable %s\n", to->str);
        exit(1);
    }

    var_val *val = malloc(sizeof(var_val));

    if(from->tok_class == VALUE) {
        if(debug) printf("from node is value\n");

        switch(from->tok_type) {
            case FLOAT:
                if(debug) printf("from node is float\n");
                val->f = atof(from->str);
                break;

            case INT:
                if(debug) printf("from node is int\n");
                val->i = atoi(from->str);
                break;

            default:
                fprintf(stderr, "Interpreter error: invalid assignment source type %d\n", from->tok_type);
                exit(1);
        }
    } else if(from->tok_class == ID) {
        var *from_var = get_variable(variables, &from->str);

        if(from_var == NULL) {
            fprintf(stderr, "Interpreter error: unable to access source variable %s\n", to->str);
            exit(1);
        }

        if(debug) printf("from node is id\n");

        switch(from->tok_type) {
            case FLOAT:
                if(debug) printf("from var is float\n");

                val->f = from_var->value.f;
                break;

            case INT:
                if(debug) printf("from var is int\n");

                val->i = from_var->value.i;
                break;

            default:
                fprintf(stderr, "Interpreter error: invalid assignment source type %d\n", from->tok_type);
                exit(1);
        }
    } else {
        fprintf(stderr, "Interpreter error: Invalid assignment source class: %d\n", from->tok_class);
        exit(1);
    }

    var* to_var = get_variable(variables, &to->str);
    printf("var name: %s\n", to_var->name);
    if(to_var == NULL) {
        fprintf(stderr, "Interpreter error: unable to access target variable %s\n", to->str);
        exit(1);
    }

    switch(to_var->type) {
        case INT:
            to_var->value.i = val->i;
            break;

        case FLOAT:
            to_var->value.f = val->f;
            break;

        default:
            fprintf(stderr, "Interpreter error: invalid assignment target variable type %d\n", to_var->type);
            exit(1);
    }
}

//Stack example:
// 1 a asg 1.23 b asg if 1 b gt { while 10 a eq not 1 a add a asg }
// ^

//Also accept variables
void interpret(sstack_t* stack, var **variables, int debug) {
    sstack_t *new_stack = init_stack();
    node_t *node;

    while((node = pop_node(stack)) != NULL) {
        if(node->tok_class == ID || node->tok_class == VALUE) {
            if(debug) printf("node is id or val\n");
            push_node(new_stack, node);
            continue;
        }

        if(node->tok_class != OPERATION) continue;

        switch(node->tok_type) {
            case ASG:
                if(debug) printf("asg operation\n");
                asg(new_stack, variables, 1);
            default:
                printf("Operation is not supported\n");

        }
    }
}