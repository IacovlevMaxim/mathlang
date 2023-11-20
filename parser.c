#include "parser.h"
#include "stack.h"
#include "tokens.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

// sstack_t *parse_block(sstack_t *stack, int debug);

int parse_op(sstack_t *stack, sstack_t *op, int debug) {

  if (stack->node->tok_class != OPERATION) {
    printf("Parser Error: Unexpected token: %s\n", stack->node->str);
    return 0;
  }

  char *op_str = stack->node->str;
  token_type_t curr_type = stack->node->tok_type;

  int val_count = 0;
  int max_val_count;

  if (stack->node->tok_type == NOT)
    max_val_count = 1;
  else
    max_val_count = 2;

  printf("n: %s v_count: %i, max_v_count: %i\n", stack->node->str, val_count,
         max_val_count);
  push_node(op, pop_node(stack));

  while (stack->node != NULL && val_count < max_val_count) {
    printf("n: %s v_count: %i, max_v_count: %i\n", stack->node->str, val_count,
           max_val_count);
    if (stack->node->tok_class == VALUE || stack->node->tok_class == ID) {
      push_node(op, pop_node(stack));
      val_count++;
    } else if (stack->node->tok_class == OPERATION) {
      parse_op(stack, op, debug);
      val_count++;
    } else {
      printf("err2\n");
      return 0;
    }
    // n = pop_node(stack);
  }
  if (val_count > max_val_count) {
    printf("Parser Error: Illegal argument count for '%s'\n", op_str);
  }
  return 1;
}

sstack_t *parse_line(sstack_t *stack, int debug) {
  sstack_t *op_line = init_stack();
  node_t *n = stack->node;
  switch (n->tok_type) {
  // case R_BRACE:
  // case L_BRACE: {
  //   push_node(op_line, pop_node(stack));
  //   return op_line;
  // }
  case ASG: {
    if (parse_op(stack, op_line, debug) == 0) {
      fprintf(stderr, "Parser Error: parse_line: parse_op failed for ASG\n");
    }
    if (debug)
      printf("leaving parse line op\n");
    return op_line;
  }
  default:
    break;
  }
  if (n->tok_class == ID)
    fprintf(stderr,
            "Parser Error: Line begins suspecioulsy with `%s` variable name. "
            "Forgot to prepend `asg`?\n",
            n->str);
  // todo print error with more context for value, variable, or operation.
  fprintf(stderr, "Parser Error: unexpected `%s`\n", n->str);
  return NULL;
}

sstack_t *parse_cond_exp(sstack_t *stack, int debug) {
  sstack_t *cond_exp = init_stack();
  node_t *cond_n = pop_node(stack);
  if (stack->node->tok_class == OPERATION)
    parse_op(stack, cond_exp, debug);
  else if (stack->node->tok_class == ID || VALUE) //...
    push_node(cond_exp, pop_node(stack));
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
        printf("Parser Notif. From parse_chunk: Entering parse_while\n");
      if (!join_stacks(parsed, parse_cond_exp(stack, debug), debug)) {
        printf("Parser Error: parse_chunk failed at WHILE case\n");
        success = 0;
        break;
      }
      if (stack->node->tok_type == L_BRACE) {
        printf("Parser Notif. From parse_chunk: Entering while block\n");
        append_node(parsed, pop_node(stack));
        if (!parse_chunk(stack, parsed, R_BRACE, debug)) {
          printf("Parser Erro: parse_chunk: failed at while block\n");
          success = 0;
          break;
        }
        if (stack->node->tok_type != R_BRACE) {
          printf("DIE\n");
          success = 0;
          break;
        }
        append_node(parsed, pop_node(stack));
      }
      break;
    }
    case IF: {
      if (debug)
        printf("Parser Notif. From parse_chunk: Entering parse_if\n");
      if (!join_stacks(parsed, parse_cond_exp(stack, debug), debug)) {
        printf("Parser Error: parse_chunk failed at IF case\n");
        success = 0;
        break;
      }
      if (stack->node->tok_type == L_BRACE) {
        printf("Parser Notif. From parse_chunk: Entering if block\n");
        append_node(parsed, pop_node(stack));
        if (!parse_chunk(stack, parsed, R_BRACE, debug)) {
          printf("Parser Error: parse_chunk: failed at if block\n");
          success = 0;
          break;
        }
        printf("Parser Notif. From parse_chunk: Leaving if block\n");
        if (stack->node->tok_type != R_BRACE) {
          printf("DIE2\n");
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
      if (stack->node->tok_type == ELSE) {
        append_node(parsed, pop_node(stack));
        // printf("misterious n->next: %p\n", pop_node(stack)->next);
        if (stack->node->tok_type == L_BRACE) {
          printf("Parser Notif. From parse_chunk: Entering else block\n");
          append_node(parsed, pop_node(stack));
          if (!parse_chunk(stack, parsed, R_BRACE, debug)) {
            printf("Parser Error: parse_chunk: failed at else block\n");
            success = 0;
            break;
          }
          if (stack->node->tok_type != R_BRACE) {
            printf("DIE3\n");
            success = 0;
            break;
          }
          append_node(parsed, pop_node(stack));
        } else {
          if (debug)
            printf(
                "Parser Notif. From parse_chunk: Entering parse_line (else)\n");
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
      // printf("bruh %s\n", stack->node->str);
      if (debug)
        printf("Parser Notif. From parse_chunk: Entering parse_line\n");
      if (!join_stacks(parsed, parse_line(stack, debug), debug)) {
        printf("Parser Error: parse_chunk failed at default case\n");
        success = 0;
        break;
      }
      printf("Parser Notif. From parse_chunk: Leaving parse_line\n");
      break;
    }
    }
    if (success == 0) {
      printf("bruh\n");
      break;
    }
  }
  return success;
}

sstack_t *parse_tokens(sstack_t *stack, int debug) {
  if (debug)
    printf("Parser Notif. Debug mode on\n");
  sstack_t *parsed = init_stack();
  parse_chunk(stack, parsed, -1, debug);
  return parsed;
}

// sstack_t *parse_cond_op(sstack_t *stack, int debug) {
//   sstack_t *cond_op = init_stack();
//   switch (stack->node->tok_type) {
//   case IF:
//   case WHILE: {
//     node_t *cond_n = pop_node(stack);
//     // push_node(cond_op, pop_node(stack));
//     //  printf("lol %s\n", stack->node->str);
//     switch (stack->node->tok_type) {
//     case NOT:
//     case EQ:
//     case GT:
//     case LT: {
//       if (parse_op(stack, cond_op, debug) == 0) {
//         fprintf(stderr, "Parser Error: parse_cond_op -> join_stacks
//         failed.\n"); return NULL;
//       }
//       break;
//     }
//     case INT: {
//       append_node(cond_op, pop_node(stack));
//       break;
//     }
//     default: {
//       fprintf(stderr, "Parser Error: Missing boolean value/operation for if
//       or "
//                       "while statement\n");
//       return NULL;
//     }
//     }
//     push_node(cond_op, cond_n);
//     if (stack->node->tok_type == R_BRACE) {
//       if (join_stacks(cond_op, parse_block(stack, debug), debug) == 0) {
//         fprintf(stderr, "Parser Error: parse_cond_op -> join_stacks
//         failed.\n"); return NULL;
//       }
//     } else {
//       if (join_stacks(cond_op, parse_line(stack, debug), debug) == 0) {
//         fprintf(stderr, "Parser Error: parse_cond_op -> join_stacks
//         failed.\n"); return NULL;
//       }
//     }
//     return cond_op;
//   }
//   case ELSE: {
//     push_node(cond_op, pop_node(stack));
//     if (stack->node->tok_type == R_BRACE) {
//       if (join_stacks(cond_op, parse_block(stack, debug), debug) == 0) {
//         fprintf(stderr, "Parser Error: parse_cond_op -> join_stacks
//         failed.\n"); return NULL;
//       }
//     } else {
//       if (join_stacks(cond_op, parse_line(stack, debug), debug) == 0) {
//         fprintf(stderr, "Parser Error: parse_cond_op -> join_stacks
//         failed.\n"); return NULL;
//       }
//     }
//     return cond_op;
//   }
//   default:
//     break;
//   }
//   fprintf(stderr,
//           "Parser Error: parse_cond_op: unhandled illegal token: `%s`.\n",
//           stack->node->str);
//   return NULL;
// }
//
// sstack_t *parse_block(sstack_t *stack, int debug) {
//   sstack_t *block = init_stack();
//   sstack_t *op_line = NULL;
//   if (stack->node->tok_type == L_BRACE) {
//     push_node(op_line, pop_node(stack));
//     if (join_stacks(block, parse_block(stack, debug), debug) == 0) {
//       fprintf(stderr, "Parser Error: parse_block -> join_stacks
//       failed.\n"); return NULL;
//     }
//   }
//   // mb problem in block depth.
//   while (stack->node != NULL && stack->node->tok_type != R_BRACE) {
//     if (debug)
//       printf("pb: %s\n", stack->node->str);
//     if (stack->node->tok_type == L_BRACE)
//       if (join_stacks(block, parse_block(stack, debug), debug) == 0) {
//         fprintf(stderr, "Parser Error: parse_block -> join_stacks
//         failed.\n"); return NULL;
//       }
//
//     if (stack->node->tok_type == IF || stack->node->tok_type == ELSE ||
//         stack->node->tok_type == WHILE) {
//       if (join_stacks(block, parse_cond_op(stack, debug), debug) == 0) {
//         fprintf(stderr, "Parser Error: parse_block -> join_stacks
//         failed.\n"); return NULL;
//       }
//     } else if (join_stacks(block, parse_line(stack, debug), debug) == 0) {
//       fprintf(stderr, "Parser Error: parse_block -> join_stacks
//       failed.\n"); return NULL;
//     }
//   }
//   if (stack->node == NULL) {
//     fprintf(stderr, "Parser Error: block not terminated with right
//     brace\n"); return NULL;
//   }
//
//   append_node(block, pop_node(stack)); // Appending `}`
//   return block;
// }
//
// sstack_t *parse_tokens(sstack_t *tokens, int debug) {
//   return parse_block(tokens, debug);
// }
