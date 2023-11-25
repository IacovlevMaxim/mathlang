#include "parser.h"
#include "stack.h"
#include "tokens.h"

#include <stdio.h>
#include <stdlib.h>

// sstack_t *parse_block(sstack_t *stack, int debug);

char *tokttstr(token_type_t t) {
  switch (t) {
  case FLOAT:
    return "float";
  case INT:
    return "int";
  default:
    return NULL;
  }
}

void nodestrtval(node_t *n) {
  // printf("%p\n", n->val);
  if (n->tok_class != VALUE)
    return;
  switch (n->tok_type) {
  case INT:
    n->val->i = strtol(n->str, NULL, 0);
    return;
  case FLOAT:
    n->val->f = strtof(n->str, NULL);
    return;
  default:
    return;
  }
}

int parse_op(sstack_t *stack, sstack_t *op, int debug) {

  if (stack->node->tok_class != OPERATION) {
    printf("Parser Error: Unexpected `%s`\n", stack->node->str);
    return -2;
  }

  if (stack->node->tok_type == ASG || stack->node->tok_type == INPUT) {
    if (stack->node->next == NULL) {
      printf("Parser Error: Unexpected end of file.\n");
      return -2;
    }
    if (stack->node->next->tok_class != ID) {
      printf("Parser Error: `%s` is nonassignable\n", stack->node->next->str);
      return -2;
    }
  }

  if (stack->node->tok_type == ASG) {
    push_node(op, pop_node(stack));
    token_type_t expd_type = stack->node->tok_type;
    char *id_str = stack->node->str;
    push_node(op, pop_node(stack));
    if (stack->node->tok_class == VALUE || stack->node->tok_class == ID) {
      if (stack->node->tok_type != expd_type) {
        printf(
            "Parser Error: attempting to assign `%s` of type %s the value `%s` "
            "of type %s\n",
            op->node->str, tokttstr(op->node->tok_type), stack->node->str,
            tokttstr(stack->node->tok_type));
        return -2;
      }
      nodestrtval(stack->node);
      push_node(op, pop_node(stack));
    } else if (stack->node->tok_class == OPERATION) {
      token_type_t res = parse_op(stack, op, debug);
      if (res == -2)
        return -2;
      if (res != expd_type) {
        printf("Parser Error: attempting to assign `%s` of type %s a return "
               "value of type %s\n",
               id_str, tokttstr(expd_type), tokttstr(res));
        return -2;
      }
    } else {
      printf("Parser Error: cannot assign `%s` to `%s`\n", stack->node->str,
             id_str);
      return -2;
    }
    return -1;
  }

  int val_count = 0;
  int max_val_count;

  token_type_t curr_type;
  token_type_t arg_type;

  int type_persists = 0;

  char *op_str = stack->node->str;
  char *op_str2;

  switch (stack->node->tok_type) {
  case DIV:
  case EQ:
  case GT:
  case LT:
  case NOT:
    type_persists = 1;
    break;
  default:
    break;
  }

  switch (stack->node->tok_type) {
  case PRINT:
  case INPUT:
    curr_type = -1;
    break;
  case DIV:
    curr_type = FLOAT;
    break;
  default:
    curr_type = INT;
  }

  switch (stack->node->tok_type) {
  case NOT:
  case INPUT:
  case PRINT:
    max_val_count = 1;
    break;
  default:
    max_val_count = 2;
  }

  if (debug)
    printf("n: %s v_count: %i, max_v_count: %i\n", stack->node->str, val_count,
           max_val_count);

  push_node(op, pop_node(stack));

  while (stack->node != NULL && val_count < max_val_count) {
    if (debug)
      printf("n: %s v_count: %i, max_v_count: %i\n", stack->node->str,
             val_count, max_val_count);
    if (stack->node->tok_class == VALUE || stack->node->tok_class == ID) {
      arg_type = stack->node->tok_type;
      nodestrtval(stack->node);
      push_node(op, pop_node(stack));
      val_count++;
    } else if (stack->node->tok_class == OPERATION) {
      op_str2 = stack->node->str;
      arg_type = parse_op(stack, op, debug);
      val_count++;
    } else {
      printf("Parser Error: unhandled token `%s`\n", stack->node->str);
      return -2;
    }

    if (!type_persists && arg_type == FLOAT && curr_type == INT)
      curr_type = FLOAT;
    else if (arg_type == -1) {
      printf("Parser Error: expected return value, but `%s` does not return "
             "anything\n",
             op_str2);
      return -2;
    } else if (arg_type == -2)
      return -2;
  }

  if (val_count < max_val_count) {
    printf("Parser Error: Unexpected end of file. `%s` expects %i arguments "
           "but got only %i\n",
           op_str, max_val_count, val_count);
    return -2;
  }
  return curr_type;
}

sstack_t *parse_line(sstack_t *stack, int debug) {
  sstack_t *op_line = init_stack();
  node_t *n = stack->node;
  switch (n->tok_type) {
  case ASG:
  case PRINT:
  case INPUT: {
    if (parse_op(stack, op_line, debug) != -1) {
      fprintf(stderr, "Parser Error: parse_line: parse_op failed for ASG, "
                      "PRINT, or INPUT\n");
      return NULL;
    }
    if (debug)
      printf("leaving parse line op\n");
    return op_line;
  }
  default:
    break;
  }
  // TODO print error with more context for value, variable, or operation.
  fprintf(stderr, "Parser Error: Unexpected `%s`\n", n->str);
  if (n->tok_class == ID)
    printf("Parser Tip: Forgot to prepend `asg`, `input` or `print`?\n");
  else if (n->tok_class == OPERATION)
    printf("Parser Tip: Forgot to prepend `print`, or assign the result to a "
           "variable?\n");
  return NULL;
}

sstack_t *parse_cond_exp(sstack_t *stack, int debug) {
  sstack_t *cond_exp = init_stack();
  node_t *cond_n = pop_node(stack);
  if (stack->node == NULL) {
    printf("Parser Error: Unexpected end of file. Expected conditional "
           "expression for `%s.\n",
           cond_n->str);
    return NULL;
  }
  if (stack->node->tok_class == OPERATION)
    parse_op(stack, cond_exp, debug);
  else if (stack->node->tok_class == ID ||
           stack->node->tok_class == VALUE) //...
    push_node(cond_exp, pop_node(stack));
  else {
    printf("Parser Error: Missing conditional expression for `%s`\n",
           cond_n->str);
    return NULL;
  }
  push_node(cond_exp, cond_n);
  return cond_exp;
}

int parse_chunk(sstack_t *stack, sstack_t *parsed, int break_point, int debug) {
  int success = 1;
  while (stack->node != NULL && stack->node->tok_type != break_point) {
    if (debug)
      printf("pb: %s\n", stack->node->str);
    switch (stack->node->tok_type) {
    case WHILE: {
      if (debug)
        printf("Parser Notif. From parse_chunk: Entering parse_cond_exp "
               "(while)\n");
      if (!join_stacks(parsed, parse_cond_exp(stack, debug), debug)) {
        printf("Parser Error: parse_cond_exp failed at WHILE case\n");
        success = 0;
        break;
      }
      if (stack->node == NULL) {
        printf("Parser Error: Unexpected end of file. Missing exec "
               "block or line for `while` statement\n");
        success = 0;
        break;
      }
      if (stack->node->tok_type == L_BRACE) {
        if (debug)
          printf("Parser Notif. From parse_chunk: Entering while block\n");
        append_node(parsed, pop_node(stack));
        if (!parse_chunk(stack, parsed, R_BRACE, debug)) {
          printf("Parser Error: parse_chunk: failed at while block\n");
          success = 0;
          break;
        }
        if (stack->node == NULL) {
          printf("Parser Error: Unexpected end of file. `while` statement's "
                 "exec block not terminated with `}`\n");
          success = 0;
          break;
        }
        // if (stack->node->tok_type != R_BRACE) {
        //   printf("DIE\n");
        //   success = 0;
        //   break;
        // }
        append_node(parsed, pop_node(stack));
      }
      break;
    }
    case IF: {
      if (debug)
        printf(
            "Parser Notif. From parse_chunk: Entering parse_cond_exp (if)\n");
      if (!join_stacks(parsed, parse_cond_exp(stack, debug), debug)) {
        printf("Parser Error: parse_cond_exp failed at IF case\n");
        success = 0;
        break;
      }
      if (stack->node == NULL) {
        printf("Parser Error: Unexpected end of file. Missing exec "
               "block or line for `if` statement\n");
        success = 0;
        break;
      }
      if (stack->node->tok_type == L_BRACE) {
        if (debug)
          printf("Parser Notif. From parse_chunk: Entering if block\n");
        append_node(parsed, pop_node(stack));
        if (!parse_chunk(stack, parsed, R_BRACE, debug)) {
          printf("Parser Error: parse_chunk: failed at if block\n");
          success = 0;
          break;
        }
        if (debug)
          printf("Parser Notif. From parse_chunk: Leaving if block\n");
        //  if (stack->node->tok_type != R_BRACE) {
        //    printf("DIE2\n");
        //    success = 0;
        //    break;
        //  }
        if (stack->node == NULL) {
          printf("Parser Error: Unexpected end of file. `if` statement's exec "
                 "block "
                 "not terminated with `}`\n");
          success = 0;
          break;
        }
        append_node(parsed, pop_node(stack));
      } else {
        if (debug)
          printf("Parser Notif. From parse_chunk: Entering parse_line (if)\n");
        if (!join_stacks(parsed, parse_line(stack, debug), debug)) {
          printf("Parser Error: parse_chunk failed at parse_line (if)\n");
          success = 0;
          break;
        }
      }
      if (stack->node == NULL)
        break;
      if (stack->node->tok_type == ELSE) {
        append_node(parsed, pop_node(stack));
        if (stack->node == NULL) {
          printf("Parser Error: Unexpected end of file. Missing exec "
                 "block or line for `else`\n");
          success = 0;
          break;
        }
        if (stack->node->tok_type == L_BRACE) {
          if (debug)
            printf("Parser Notif. From parse_chunk: Entering else block\n");
          append_node(parsed, pop_node(stack));
          if (!parse_chunk(stack, parsed, R_BRACE, debug)) {
            printf("Parser Error: parse_chunk: failed at else block\n");
            success = 0;
            break;
          }
          // if (stack->node->tok_type != R_BRACE) {
          //   printf("DIE3\n");
          //   success = 0;
          //   break;
          // }
          if (stack->node == NULL) {
            printf(
                "Parser Error: Unexpected end of file. `if` statement's exec "
                "block "
                "not terminated with `}`\n");
            success = 0;
            break;
          }
          append_node(parsed, pop_node(stack));
        } else {
          if (debug)
            printf("Parser Notif. From parse_chunk: Entering parse_line "
                   "(else)\n");
          if (!join_stacks(parsed, parse_line(stack, debug), debug)) {
            printf("Parser Error: parse_chunk failed at parse_line (else)\n");
            success = 0;
            break;
          }
        }
      }
      break;
    }
    default: {
      if (debug)
        printf("Parser Notif. From parse_chunk: Entering parse_line\n");
      if (!join_stacks(parsed, parse_line(stack, debug), debug)) {
        printf("Parser Error: parse_chunk failed at default case\n");
        success = 0;
        break;
      }
      if (debug)
        printf("Parser Notif. From parse_chunk: Leaving parse_line\n");
      break;
    }
    }
    if (success == 0) {
      if (debug)
        printf("Parser Msg: Exiting parser loop on encounter of error(s)\n");
      break;
    }
  }
  return success;
}

sstack_t *parse_tokens(sstack_t *stack, int debug) {
  if (debug)
    printf("Parser Notif. Debug mode on\n");
  if (stack->node == NULL) {
    printf("Parser Error: Main block is empty!\n");
    return NULL;
  }
  sstack_t *parsed = init_stack();
  parse_chunk(stack, parsed, -1, debug);
  return parsed;
}
