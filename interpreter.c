#include "lexer.h"
#include "math.h"
#include "stack.h"
#include "string.h"
#include "tokens.h"
#include <stdio.h>
#include <stdlib.h>

const double EQ_PRECISION = 0.001;

var *get_variable(var **variables, char **name) {
  var *currVar;
  int i = 0;
  while ((currVar = (*variables + i))->type) {
    if (strcmp(currVar->name, *name) == 0) {
      return currVar;
    }
    i++;
  }

  return NULL;
}

int var_lookup_int(var *vars, char *s) {
  var *curr_var = get_variable(&vars, &s);
  return curr_var->value.i;
}

float var_lookup_float(var *vars, char *s) {
  var *curr_var = get_variable(&vars, &s);
  return curr_var->value.f;
}

float get_float_value(var *vars, node_t *node) {
  switch (node->tok_class) {
  case VALUE: {
    if (node->tok_type == FLOAT) {
      return node->val.f;
    }
    return (float)node->val.i;
  }
  case ID: {
    if (node->tok_type == FLOAT) {
      return var_lookup_float(vars, node->str);
    }
    return (float)var_lookup_int(vars, node->str);
  }
  default:
    return 0;
  }
}

int get_int_value(var *vars, node_t *node) {
  switch (node->tok_class) {
  case VALUE: {
    if (node->tok_type == FLOAT) {
      return (int)node->val.f;
    }
    return node->val.i;
  }
  case ID: {
    if (node->tok_type == FLOAT) {
      return (int)var_lookup_float(vars, node->str);
    }
    return var_lookup_int(vars, node->str);
  }
  default:
    return 0;
  }
}

int get_bool(var *vars, node_t *node) { return get_int_value(vars, node); }

// TODO rewrite, break in funcs.

void asg(sstack_t *params, var *vars, int debug) {
  node_t *id_node = pop_node(params);
  node_t *val_node = pop_node(params);
  var *var = get_variable(&vars, &id_node->str);
  if (debug)
    printf("var name: %s\nvar val: ", var->name);
  switch (var->type) {
  case INT: {
    var->value.i = get_int_value(vars, val_node);
    if (debug)
      printf("%i\n", var->value.i);
    break;
  }
  case FLOAT: {
    var->value.f = get_float_value(vars, val_node);
    if (debug)
      printf("%f\n", var->value.f);
    break;
  }
  default:
    exit(1);
  }
  free(id_node);
  free(val_node);
}

void mul(sstack_t *params, var *vars, int debug) {
  node_t *n1 = pop_node(params);
  node_t *n2 = pop_node(params);

  node_t *res = init_node();
  res->tok_class = VALUE;
  res->tok_type = FLOAT;

  if (n1->tok_type == INT && n2->tok_type == INT) {
    res->tok_type = INT;
    res->val.i = get_int_value(vars, n1) * get_int_value(vars, n2);
  } else
    res->val.f = get_float_value(vars, n1) * get_float_value(vars, n2);
  push_node(params, res);
  free(n1);
  free(n2);
}

void divv(sstack_t *params, var *vars, int debug) {
  if (debug)
    printf("Started divv\n");
  node_t *n1 = pop_node(params);
  node_t *n2 = pop_node(params);

  node_t *res = init_node();
  res->tok_class = VALUE;
  res->tok_type = FLOAT;

  res->val.f = get_float_value(vars, n1) / get_float_value(vars, n2);
  push_node(params, res);
  free(n1);
  free(n2);
}

void add(sstack_t *params, var *vars, int debug) {
  if (debug)
    printf("Started and\n");
  node_t *n1 = pop_node(params);
  node_t *n2 = pop_node(params);

  node_t *res = init_node();
  res->tok_class = VALUE;
  res->tok_type = FLOAT;

  if (n1->tok_type == INT && n2->tok_type == INT) {
    res->tok_type = INT;
    res->val.i = get_int_value(vars, n1) + get_int_value(vars, n2);
  } else
    res->val.f = get_float_value(vars, n1) + get_float_value(vars, n2);
  push_node(params, res);
  free(n1);
  free(n2);
}

void sub(sstack_t *params, var *vars, int debug) {
  if (debug)
    printf("Started sub\n");
  node_t *n1 = pop_node(params);
  node_t *n2 = pop_node(params);

  node_t *res = init_node();
  res->tok_class = VALUE;
  res->tok_type = FLOAT;

  if (n1->tok_type == INT && n2->tok_type == INT) {
    res->tok_type = INT;
    res->val.i = get_int_value(vars, n1) - get_int_value(vars, n2);
  } else
    res->val.f = get_float_value(vars, n1) - get_float_value(vars, n2);
  push_node(params, res);
  free(n1);
  free(n2);
}

void eq(sstack_t *params, var *vars, int debug) {
  if (debug)
    printf("Started eq\n");

  node_t *n1 = pop_node(params);
  node_t *n2 = pop_node(params);

  node_t *res = init_node();
  res->tok_class = VALUE;
  res->tok_type = INT;

  res->val.i = fabsl(get_float_value(vars, n1) - get_float_value(vars, n2)) <
               EQ_PRECISION;

  push_node(params, res);
  free(n1);
  free(n2);
}

void not(sstack_t * params, var *vars, int debug) {
  if (debug)
    printf("Started not\n");
  node_t *p = pop_node(params);

  node_t *n = init_node();
  n->tok_class = VALUE;
  n->tok_type = INT;
  n->val.i = get_bool(vars, p) == 0;
  push_node(params, n);
  free(p);
}

void gt(sstack_t *params, var *vars, int debug) {
  if (debug)
    printf("Started gt\n");

  node_t *from = pop_node(params);
  node_t *to = pop_node(params);

  node_t *res = init_node();
  res->tok_class = VALUE;
  res->tok_type = INT;

  res->val.i = get_float_value(vars, to) < get_float_value(vars, from);

  push_node(params, res);
  free(from);
  free(to);
}

void lt(sstack_t *params, var *vars, int debug) {
  if (debug)
    printf("Started lt\n");

  node_t *from = pop_node(params);
  node_t *to = pop_node(params);

  node_t *res = init_node();
  res->tok_class = VALUE;
  res->tok_type = INT;

  res->val.i = get_float_value(vars, to) > get_float_value(vars, from);

  push_node(params, res);
  free(from);
  free(to);
}

void and (sstack_t * params, var *vars, int debug) {
  if (debug)
    printf("Started and\n");

  node_t *from = pop_node(params);
  node_t *to = pop_node(params);

  node_t *res = init_node();
  res->tok_class = VALUE;
  res->tok_type = INT;

  res->val.i = get_bool(vars, to) && get_bool(vars, from);

  push_node(params, res);
  free(from);
  free(to);
}

void or (sstack_t * params, var *vars, int debug) {
  if (debug)
    printf("Started or\n");

  node_t *from = pop_node(params);
  node_t *to = pop_node(params);

  node_t *res = init_node();
  res->tok_class = VALUE;
  res->tok_type = INT;

  res->val.i = get_bool(vars, to) || get_bool(vars, from);

  push_node(params, res);
  free(from);
  free(to);
}

void print(sstack_t *params, var *vars, int debug) {
  if (debug)
    printf("Started print\n");
  node_t *n = pop_node(params);
  if (n->tok_class == ID && debug)
    printf("%s: ", n->str);
  if (n->tok_type == INT)
    printf("%i\n", get_int_value(vars, n));
  else
    printf("%f\n", get_float_value(vars, n));

  free(n);
}

void input(sstack_t *params, var *vars, int debug) {
  node_t *n = pop_node(params);
  var *var = get_variable(&vars, &n->str);
  /// somehow read with fgets and then like in asg futher
  // ex: sscanf(buff, "%i", var.value.i)
}

// Stack example:
//  1 a asg 1.23 b asg if 1 b gt { while 10 a eq not 1 a add a asg }
//  ^

const node_t *eval_op(const node_t *op, sstack_t *new_stack, var *vars,
                      int debug) {
  switch (op->tok_type) {
  case ASG:
    asg(new_stack, vars, debug);
    break;
  case ADD:
    add(new_stack, vars, debug);
    break;
  case SUB:
    sub(new_stack, vars, debug);
    break;
  case MUL:
    mul(new_stack, vars, debug);
    break;
  case DIV:
    divv(new_stack, vars, debug);
    break;
  case EQ:
    eq(new_stack, vars, debug);
    break;
  case GT:
    gt(new_stack, vars, debug);
    break;
  case LT:
    lt(new_stack, vars, debug);
    break;
  case NOT:
    not(new_stack, vars, debug);
    break;
  case AND:
    and(new_stack, vars, debug);
    break;
  case OR:
    or (new_stack, vars, debug);
    break;
  case PRINT:
    print(new_stack, vars, debug);
    break;
  case INPUT:
    input(new_stack, vars, debug);
    break;
  default:
    printf("Operation is not supported: %s\n", op->str);
    break;
  }
  return op->next;
}

const node_t *eval_cond(const node_t *node, sstack_t *new_stack, var *vars,
                        int debug) {
  while (node != NULL && node->tok_type != ASG && node->tok_type != PRINT &&
         node->tok_type != INPUT) {
    switch (node->tok_class) {
    case ID:
    case VALUE: {
      if (debug)
        printf("cond val: %s\n", node->str);
      push_node(new_stack, dup_node(node));
      break;
    }
    case PUNCT:
      return node;
    default: {
      if (debug)
        printf("assumed cond op: %s\n", node->str);
      eval_op(node, new_stack, vars, debug);
      // printf("----------- %i\n", new_stack->node->val.i);
    }
    }
    node = node->next;
  }
  return node;
}

const node_t *eval_block(const node_t *node, sstack_t *new_stack, var *vars,
                         int debug);

const node_t *skip_block(const node_t *node) {
  while (node->tok_type != R_BRACE)
    node = node->next;
  return node->next;
}

const node_t *eval_if(const node_t *node, sstack_t *new_stack, var *vars,
                      int debug) {
  node = node->next;
  node = eval_cond(node, new_stack, vars, debug);
  if (debug)
    printf("IF cond bool: %i\n", new_stack->node->val.i);
  node = node->next;
  if (new_stack->node->val.i)
    node = eval_block(node, new_stack, vars, debug);
  else
    node = skip_block(node);
  if (node != NULL && node->tok_type == ELSE) {
    node = node->next->next;
    if (!new_stack->node->val.i) {
      node = eval_block(node, new_stack, vars, debug);
      node = node->next;
    } else
      node = skip_block(node);
  }
  free(pop_node(new_stack));
  return node;
}

const node_t *eval_while(const node_t *node, sstack_t *new_stack, var *vars,
                         int debug) {
  const node_t *label = node;
  node = node->next;
  node = eval_cond(node, new_stack, vars, debug);
  node = node->next;
  if (debug)
    printf("WHILE cond bool: %i\n", new_stack->node->val.i);
  if (new_stack->node->val.i) {
    node = eval_block(node, new_stack, vars, debug);
    free(pop_node(new_stack));
    return label;
  } else
    node = skip_block(node);
  free(pop_node(new_stack));
  return node;
}

const node_t *eval_block(const node_t *node, sstack_t *new_stack, var *vars,
                         int debug) {
  if (debug)
    printf("ENTERED BLOCK\n");
  while (node != NULL && node->tok_type != R_BRACE) {
    if (node->tok_class == ID || node->tok_class == VALUE) {
      if (debug)
        printf("node is id or val\n");
      push_node(new_stack, dup_node(node));
      node = node->next;
    } else if (node->tok_class == COND_OP) {
      if (node->tok_type == IF) {
        node = eval_if(node, new_stack, vars, debug);
      } else {
        // printf("----> %s\n", node->str);
        node = eval_while(node, new_stack, vars, debug);
      }
    } else {
      if (debug)
        printf("assumed node is op\n");
      node = eval_op(node, new_stack, vars, debug);
    }
  }
  return node;
}

void interpret(sstack_t *stack, var *vars, int debug) {
  sstack_t *new_stack = init_stack();
  const node_t *node = eval_block(stack->node, new_stack, vars, debug);
}
