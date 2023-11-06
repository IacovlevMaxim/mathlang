//
// Created by maxim on 06.11.2023.
//

#ifndef MATHLANG_STACK_H
#define MATHLANG_STACK_H
#include "lexer.h"

typedef union {
    float *f;
    int *i;
} node_val_t;

typedef struct node_t {
    token_class_t tok_class;
    token_type_t tok_type;
    char *str;
    // size_t no_line;
    node_val_t *val;
    struct node_t *next;
} node_t;

typedef struct sstack_t {
    node_t *node;
    node_t *last;
} sstack_t;

node_t *init_node();
int push_node(sstack_t *stack, node_t *to_push);
node_t *pop_node(sstack_t *stack);
void append_node(node_t *top, node_t *new);

#endif //MATHLANG_STACK_H
