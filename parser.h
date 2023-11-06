#ifndef MATHLANG_PARSER_H
#define MATHLANG_PARSER_H

#include <stddef.h>
#include <stdlib.h>
#include "stack.h"

void parse(sstack_t *exec_stack, sstack_t *def_stack, node_t *tokens[],
           size_t size);

#endif // MATHLANG_PARSER_H