#include "lexer.h"
#include "math.h"
#include "stack.h"
#include "string.h"
#include "tokens.h"
#include <stdio.h>
#include <stdlib.h>

const double EQ_PRECISION = 0.001;

/////// TODO
void eval_while();
void eval_if();

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
      return node->val->f;
    }
    return (float)node->val->i;
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
      return (int)node->val->f;
    }
    return node->val->i;
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

int get_bool(var *vars, node_t *node) { return get_int_value(vars, node) == 0; }

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
    res->val->i = get_int_value(vars, n1) * get_int_value(vars, n2);
  } else
    res->val->f = get_float_value(vars, n1) * get_float_value(vars, n2);
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

  res->val->f = get_float_value(vars, n1) / get_float_value(vars, n2);
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
    res->val->i = get_int_value(vars, n1) + get_int_value(vars, n2);
  } else
    res->val->f = get_float_value(vars, n1) + get_float_value(vars, n2);
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
    res->val->i = get_int_value(vars, n1) - get_int_value(vars, n2);
  } else
    res->val->f = get_float_value(vars, n1) - get_float_value(vars, n2);
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

  res->val->i = fabsl(get_float_value(vars, n1) - get_float_value(vars, n2)) <
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
  n->val->i = !get_bool(vars, n);
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

  res->val->i = get_float_value(vars, to) < get_float_value(vars, from);

  push_node(params, res);
  free(from);
  free(to);
}

void lt(sstack_t *params, var *vars, int debug) {
  if (debug)
    printf("Started lt\n");
  gt(params, vars, debug);
  not(params, vars, debug);
}

void and (sstack_t * params, var *vars, int debug) {
  if (debug)
    printf("Started and\n");

  node_t *from = pop_node(params);
  node_t *to = pop_node(params);

  node_t *res = init_node();
  res->tok_class = VALUE;
  res->tok_type = INT;

  res->val->i = get_bool(vars, to) && get_bool(vars, from);

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

  res->val->i = get_bool(vars, to) || get_bool(vars, from);

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

void eval_op(node_t *op, sstack_t *new_stack, var *vars, int debug) {
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
    printf("Operation is not supported\n");
    break;
  }
  free(op);
}

void interpret(sstack_t *stack, var *vars, int debug) {
  sstack_t *new_stack = init_stack();
  node_t *node;

  while ((node = pop_node(stack)) != NULL) {
    if (node->tok_class == ID || node->tok_class == VALUE) {
      if (debug)
        printf("node is id or val\n");
      push_node(new_stack, node);
      continue;
    }

    if (node->tok_class == COND_OP) {
      if (node->tok_type == IF) {
        printf("IF: UNIMPLEMENTED\n");
        // eval_if(...) TODO
      } else {
        printf("WHILE: UNIMPLEMENTED\n");
        // eval_while(...) TODO
      }
    }

    if (node->tok_class != OPERATION) {
      free(node);
      continue;
    }

    if (debug)
      printf("node is op\n");
    eval_op(node, new_stack, vars, debug);
  }
}

/////// old funcs
///
// node_t *add(sstack_t *params, var *vars, int debug) {
//   if (debug)
//     printf("Started add\n");
//   node_t *to = pop_node(params);
//   node_t *from = pop_node(params);
//
//   if (debug)
//     printf("Popped nodes\n");
//   node_t *res = init_node();
//   res->tok_class = VALUE;
//
//   if (debug)
//     printf("Created res node\n");
//   if (to->tok_type == INT && from->tok_type == INT) {
//     if (debug)
//       printf("int int\n");
//     res->tok_type = INT;
//     res->val->i = to->val->i + from->val->i;
//   } else if (to->tok_type == FLOAT || from->tok_type == FLOAT) {
//     if (debug) {
//       printf("float int\n");
//       printf("to f: %f\n", get_float_value(vars, to));
//       printf("from int: %f\n", get_float_value(vars, from));
//     }
//
//     res->tok_type = FLOAT;
//     res->val->f = get_float_value(vars, to) + get_float_value(vars, from);
//     if (debug)
//       printf("res f: %f\n", res->val->f);
//   } else {
//     fprintf(stderr, "Interpreter error: invalid argument types (neither
//     floats "
//                     "nor ints)\n");
//     exit(1);
//   }
//
//   return res;
// }
//
// node_t *sub(sstack_t *params, var *vars, int debug) {
//   if (debug)
//     printf("Started dec\n");
//   node_t *node = params->node;
//   if (node->tok_type == INT) {
//     node->val->i *= -1;
//   } else if (node->tok_type == FLOAT) {
//     node->val->f *= -1;
//   } else {
//     fprintf(
//         stderr,
//         "Interpreter error: invalid argument type (neither floats nor
//         ints)\n");
//     exit(1);
//   }
//
//   return add(params, vars, debug);
// }
//
// node_t *mul(sstack_t *params, var *vars, int debug) {
//   if (debug)
//     printf("Started mul\n");
//   node_t *to = pop_node(params);
//   node_t *from = pop_node(params);
//
//   if (debug)
//     printf("Popped nodes\n");
//   node_t *res = init_node();
//   res->tok_class = VALUE;
//
//   if (debug)
//     printf("Created res node\n");
//   if (to->tok_type == INT && from->tok_type == INT) {
//     if (debug)
//       printf("int int\n");
//     res->tok_type = INT;
//     res->val->i = to->val->i * from->val->i;
//   } else if (to->tok_type == FLOAT || from->tok_type == FLOAT) {
//     if (debug) {
//       printf("float int\n");
//       printf("to f: %f\n", get_float_value(vars, to));
//       printf("from int: %f\n", get_float_value(vars, from));
//     }
//
//     res->tok_type = FLOAT;
//     res->val->f = get_float_value(vars, to) * get_float_value(vars, from);
//     if (debug)
//       printf("res f: %f\n", res->val->f);
//   } else {
//     fprintf(stderr, "Interpreter error: invalid argument types (neither
//     floats "
//                     "nor ints)\n");
//     exit(1);
//   }
//
//   return res;
// }
//
// node_t *divv(sstack_t *params, var *vars, int debug) {
//   if (debug)
//     printf("Started dec\n");
//   node_t *node = params->node;
//   if (node->tok_type == INT || node->tok_type == FLOAT) {
//     float val = get_float_value(vars, node);
//     if (val == 0) {
//       fprintf(stderr, "Interpreter error: division by zero\n");
//       exit(1);
//     }
//     node->val->f = 1 / val;
//   } else {
//     fprintf(
//         stderr,
//         "Interpreter error: invalid argument type (neither floats nor
//         ints)\n");
//     exit(1);
//   }
//
//   return mul(params, vars, debug);
// }
//
// void asg(sstack_t *params, var **variables, int debug) {
//   node_t *to = pop_node(params);
//   node_t *from = pop_node(params);
//
//   if (to->tok_class != ID) {
//     fprintf(stderr,
//             "Interpreter error: assignment target is not a variable: %s\n",
//             to->str);
//     exit(1);
//   }
//
//   if (to->tok_type == INT && from->tok_type == FLOAT) {
//     fprintf(stderr, "Interpreter error: assigning float to int variable
//     %s\n",
//             to->str);
//     exit(1);
//   }
//
//   var_val *val = malloc(sizeof(var_val));
//
//   if (from->tok_class == VALUE) {
//     if (debug)
//       printf("from node is value\n");
//
//     switch (from->tok_type) {
//     case FLOAT:
//       if (debug)
//         printf("from node is float\n");
//       val->f = from->val->f;
//       break;
//
//     case INT:
//       if (debug)
//         printf("from node is int\n");
//       val->i = from->val->i;
//       break;
//
//     default:
//       fprintf(stderr, "Interpreter error: invalid assignment source type
//       %d\n",
//               from->tok_type);
//       exit(1);
//     }
//   } else if (from->tok_class == ID) {
//     var *from_var = get_variable(variables, &from->str);
//
//     if (from_var == NULL) {
//       fprintf(stderr,
//               "Interpreter error: unable to access source variable %s\n",
//               to->str);
//       exit(1);
//     }
//
//     if (debug)
//       printf("from node is id\n");
//
//     switch (from->tok_type) {
//     case FLOAT:
//       if (debug)
//         printf("from var is float\n");
//
//       val->f = from_var->value.f;
//       break;
//
//     case INT:
//       if (debug)
//         printf("from var is int\n");
//
//       val->i = from_var->value.i;
//       break;
//
//     default:
//       fprintf(stderr, "Interpreter error: invalid assignment source type
//       %d\n",
//               from->tok_type);
//       exit(1);
//     }
//   } else {
//     fprintf(stderr, "Interpreter error: Invalid assignment source class:
//     %d\n",
//             from->tok_class);
//     exit(1);
//   }
//
//   var *to_var = get_variable(variables, &to->str);
//   printf("var name: %s\n", to_var->name);
//   if (to_var == NULL) {
//     fprintf(stderr, "Interpreter error: unable to access target variable
//     %s\n",
//             to->str);
//     exit(1);
//   }
//
//   switch (to_var->type) {
//   case INT:
//     to_var->value.i = val->i;
//     break;
//
//   case FLOAT:
//     to_var->value.f = val->f;
//     break;
//
//   default:
//     fprintf(stderr,
//             "Interpreter error: invalid assignment target variable type
//             %d\n", to_var->type);
//     exit(1);
//   }
//
//   free(to);
//   free(from);
// }
