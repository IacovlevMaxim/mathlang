//
// Created by maxim on 07.11.2023.
//

#ifndef MATHLANG_TOKENS_H
#define MATHLANG_TOKENS_H
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
    // OF CONSOLE MANIPULAITON:
    PRINT,
    INPUT,
    // OF PUNCT:
    L_BRACE,
    R_BRACE,
} token_type_t;
typedef enum { CLASS_NONE, VALUE, ID, DEF_OP, OPERATION, COND_OP, PUNCT } token_class_t;
#endif //MATHLANG_TOKENS_H
