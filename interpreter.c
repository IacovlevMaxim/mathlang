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

float get_float_value(node_t* node) {
    if(node->tok_type == FLOAT) {
        return node->val->f;
    }

    return (float) node->val->i;
}

void asg(sstack_t *params, var **variables, int debug) {
    node_t *to = pop_node(params);
    node_t *from = pop_node(params);

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

    free(to);
    free(from);
}

node_t* add(sstack_t *params, int debug) {
    if(debug) printf("Started add\n");
    node_t *to = pop_node(params);
    node_t *from = pop_node(params);

    if(debug) printf("Popped nodes\n");
    node_t *res = init_node();
    res->tok_class = VALUE;

    if(debug) printf("Created res node\n");
    if(to->tok_type == INT && from->tok_type == INT) {
        if(debug) printf("int int\n");
        res->tok_type = INT;
        res->val->i = to->val->i + from->val->i;
    } else if(to->tok_type == FLOAT || from->tok_type == FLOAT) {
        if(debug) {
            printf("float int\n");
            printf("to f: %f\n", get_float_value(to) );
            printf("from int: %f\n", get_float_value(from));
        }

        res->tok_type = FLOAT;
        res->val->f = get_float_value(to) + get_float_value(from);
        if(debug) printf("res f: %f\n", res->val->f);
    } else {
        fprintf(stderr, "Interpreter error: invalid argument types (neither floats nor ints)\n");
        exit(1);
    }

    return res;
}

node_t* sub(sstack_t *params, int debug) {
    if(debug) printf("Started dec\n");
    node_t* node = params->node;
    if(node->tok_type == INT) {
        node->val->i *= -1;
    } else if(node->tok_type == FLOAT) {
        node->val->f *= -1;
    } else {
        fprintf(stderr, "Interpreter error: invalid argument type (neither floats nor ints)\n");
        exit(1);
    }

    return add(params, debug);
}

node_t* mul(sstack_t *params, int debug) {
    if(debug) printf("Started mul\n");
    node_t *to = pop_node(params);
    node_t *from = pop_node(params);

    if(debug) printf("Popped nodes\n");
    node_t *res = init_node();
    res->tok_class = VALUE;

    if(debug) printf("Created res node\n");
    if(to->tok_type == INT && from->tok_type == INT) {
        if(debug) printf("int int\n");
        res->tok_type = INT;
        res->val->i = to->val->i * from->val->i;
    } else if(to->tok_type == FLOAT || from->tok_type == FLOAT) {
        if(debug) {
            printf("float int\n");
            printf("to f: %f\n", get_float_value(to) );
            printf("from int: %f\n", get_float_value(from));
        }

        res->tok_type = FLOAT;
        res->val->f = get_float_value(to) * get_float_value(from);
        if(debug) printf("res f: %f\n", res->val->f);
    } else {
        fprintf(stderr, "Interpreter error: invalid argument types (neither floats nor ints)\n");
        exit(1);
    }

    return res;
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
                break;
            case ADD:
                if(debug) printf("add operation\n");
                node_t *add_res = add(new_stack, 1);
                push_node(new_stack, add_res);
//                printf("add res value: %f\n", pop_node(new_stack)->val->f);
                break;
            case SUB:
                if(debug) printf("sub operation\n");
                node_t *sub_res = sub(new_stack, 1);
                push_node(new_stack, sub_res);
                printf("dec res value: %f\n", pop_node(new_stack)->val->f);
                break;
            case MUL:
                if(debug) printf("sub operation\n");
                node_t *mul_res = mul(new_stack, 1);
                push_node(new_stack, mul_res);
                printf("mul res value: %f\n", pop_node(new_stack)->val->f);
                break;
            default:
                printf("Operation is not supported\n");
                break;

        }
    }
}