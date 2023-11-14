//
// Created by maxim on 03.11.2023.
//

#ifndef MATHLANG_LEXER_H
#define MATHLANG_LEXER_H

#include "stack.h"
#define MAX_VAR_AMOUNT 10

int is_integer(char *str);
int is_float(char *str);
int get_op_type(char *str);

typedef union {
  float f;
  int i;
} var_val;

typedef struct var {
  char *name;
  int type;
  var_val value;
} var;

void tokenize(char *code, sstack_t *top, var **variables, int debug);

#endif // MATHLANG_LEXER_H

// typedef enum { INT, FLOAT } type_t;
// typedef enum { ASG, ADD, SUB, DIV, MUL } num_ops_t;
// typedef enum { EQ, GT, LT, NOT, AND, OR } bool_ops_t;
// typedef enum { WHILE, IF } cond_ops_t;
// typedef enum { L_BRACE, R_BRACE } punct_t;
