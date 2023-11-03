//
// Created by maxim on 03.11.2023.
//

#ifndef MATHLANG_LEXER_H
#define MATHLANG_LEXER_H

typedef enum {
    INT,
    FLOAT
} type_t;

typedef enum {
    ASG,
    ADD,
    SUB,
    DIV,
    MUL
} num_ops_t;

typedef enum {
    EQ,
    GT,
    LT,
    NOT,
    AND,
    OR
} bool_ops_t;

typedef enum {
    WHILE,
    IF
} cond_ops_t;

typedef enum {
    L_BRACE,
    R_BRACE
} punct_t;

#endif //MATHLANG_LEXER_H
