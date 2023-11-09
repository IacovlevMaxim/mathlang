#ifndef MATHLANG_PARSER_H
#define MATHLANG_PARSER_H

#include "stack.h"

// void parse(sstack_t *exec_stack, sstack_t *def_stack, node_t *tokens[],
//            size_t size);

sstack_t *parse_op(sstack_t *stack, int debug);
sstack_t *parse_main(sstack_t *stack, int debug);
sstack_t *parse_all(sstack_t *stack, int debug);

#endif // MATHLANG_PARSER_H
