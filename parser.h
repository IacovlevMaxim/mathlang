#ifndef MATHLANG_PARSER_H
#define MATHLANG_PARSER_H

#include "lexer.h"
#include <stddef.h>
#include <stdlib.h>

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
} sstack_t;

node_t *init_node();
int push_node(sstack_t *stack, node_t *to_push);
node_t *pop_node(sstack_t *stack);
void parse(sstack_t *exec_stack, sstack_t *def_stack, node_t *tokens[],
           size_t size);

#endif // MATHLANG_PARSER_H

// typedef struct {
//   node_val_t *vals;
//   size_t size;
//   size_t pos;
//   char **names;
// } registr_t;

// typedef struct {
//   size_t size;
//   stack_t **branches;
// } tree_t;
