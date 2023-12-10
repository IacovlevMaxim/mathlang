#ifndef MATHLANG_INTERPRETER_H
#define MATHLANG_INTERPRETER_H
#include "stack.h"

typedef struct var var;

void interpret(sstack_t *stack, var *variables, int debug);

#endif // MATHLANG_INTERPRETER_H
