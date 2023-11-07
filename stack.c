#include "stack.h"
#include <stdlib.h>

node_t *init_node() {
  node_t *node;
  node = malloc(sizeof(node_t));
  if (node == NULL)
    return NULL;
  // node->data = alloc_data_of(type);
  node->next = NULL;
  return node;
}

int push_node(sstack_t *stack, node_t *to_push) {
  if (to_push == NULL || stack == NULL)
    return 0;
  // printf("%s\n", to_push->str);
  to_push->next = stack->node;
  stack->node = to_push;
  return 1;
}

node_t *pop_node(sstack_t *stack) {
  node_t *to_pop = stack->node;
  if (stack->node == NULL)
    return NULL;
  stack->node = stack->node->next;
  // printf("%s\n", to_pop->str);
  return to_pop;
}

node_t *get_last_node(sstack_t *stack) {
    if(stack->node == NULL) return NULL;

    node_t *n = stack->node;
    while(n->next != NULL) {
        n = n->next;
    }

    return n;
}

// TODO for rewrite to be possible,
// nodes must have besides `next` the `prev` field
// and basicaly this function should
// do the push operation in reverse..
int append_node(sstack_t *top, node_t *nnew) {
  node_t *last = get_last_node(top);
  if(last == NULL) return 0;

  last->next = nnew;
  return 1;
}

sstack_t *init_stack() {
  sstack_t *stack = malloc(sizeof(sstack_t));
  if (stack == NULL)
    return NULL;
  stack->node = NULL;
  return stack;
}

int join_stacks(sstack_t *s1, sstack_t *s2) {
  if (s1 == NULL || s2 == NULL || s2->node == NULL)
    return 0;

  node_t *last = get_last_node(s1);
  last->next = s2->node;
  return 1;
}
