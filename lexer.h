//
// Created by maxim on 03.11.2023.
//

#ifndef MATHLANG_LEXER_H
#define MATHLANG_LEXER_H

#include "parser.h"

typedef struct node_t node_t;

int is_integer(char* str);
int is_float(char* str);
int get_op_type(char* str);

typedef enum {
  // For error handling
  TYPE_NONE,
  // OF VALUE:
  INT,
  FLOAT,
  // OF DEF_OP:
  DEF_INT,
  DEF_FLOAT,
  // OF OPERATION:
  ASG,
  ADD,
  SUB,
  DIV,
  MUL,
  EQ,
  GT,
  LT,
  NOT,
  AND,
  OR,
  // OF COND_OP:
  WHILE,
  IF,
  ELSE,
  // OF PUNCT:
  L_BRACE,
  R_BRACE,
} token_type_t;

typedef struct var {
    char* name;
    int type;
} var;

typedef enum { CLASS_NONE, VALUE, ID, DEF_OP, OPERATION, COND_OP, PUNCT } token_class_t;

void parse_code(char* code, node_t* top);
// Tokens will be stored in an array, eg. node_t* tokens[], so the parser can
// traverse them easily. node_t is in parser.h. the `next` and `val` field
// should be left alone.

#endif // MATHLANG_LEXER_H

// typedef enum { INT, FLOAT } type_t;
// typedef enum { ASG, ADD, SUB, DIV, MUL } num_ops_t;
// typedef enum { EQ, GT, LT, NOT, AND, OR } bool_ops_t;
// typedef enum { WHILE, IF } cond_ops_t;
// typedef enum { L_BRACE, R_BRACE } punct_t;
