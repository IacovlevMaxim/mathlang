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

// TODO for rewrite to be possible,
// nodes must have besides `next` the `prev` field
// and basicaly this function should
// do the push operation in reverse..
void append_node(node_t *top, node_t *nnew) {
  if (top->tok_class == CLASS_NONE) {
    //        printf("added as first: %s\n", new->str);
    *top = *nnew;
    return;
  }

  if (top->next == NULL) {
    //        printf("added second after %s: %s\n", top->str, new->str);
    top->next = nnew;

    //        printf("%s\n", top->next->str);
    return;
  }

  node_t *curr = top;
  while (curr->next != NULL) {
    //        printf("skipping %s\n", curr->str);
    curr = curr->next;
  }

  //    printf("added new after %s: %s\n", curr->str, new->str);
  curr->next = nnew;
}

sstack_t *init_stack() {
  sstack_t *stack = malloc(sizeof(sstack_t));
  if (stack == NULL)
    return NULL;
  stack->node = NULL;
  stack->last = stack->node;
  return stack;
}

int join_stacks(sstack_t *s1, sstack_t *s2) {
  if (s1 == NULL || s2 == NULL || s2->node == NULL)
    return NULL;
  s1->last = s2->node;
}
