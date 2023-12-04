//
// Created by maxim on 06.11.2023.
//

#ifndef MATHLANG_STACK_H
#define MATHLANG_STACK_H
#include "tokens.h"

typedef union {
  float f;
  int i;
} node_val_t;

typedef struct node_t {
  token_class_t tok_class;
  token_type_t tok_type;
  // size_t no_line;
  node_val_t val;
  char *str;
  struct node_t *next;
} node_t;

typedef struct sstack_t {
  node_t *node;
} sstack_t;

node_t *init_node();
sstack_t *init_stack();
int push_node(sstack_t *stack, node_t *to_push);
node_t *pop_node(sstack_t *stack);
int append_node(sstack_t *top, node_t *nnew);
node_t *dup_node(const node_t *n);

int join_stacks(sstack_t *s1, sstack_t *s2, int debug);

#endif // MATHLANG_STACK_H
