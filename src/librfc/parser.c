#include "parser.h"
#include <stdio.h>

// see samples/main.g for the grammar

result_stack_check stack_check(stack_ast_node* stack, check c[], int n) {
  check_result result = {0};
  if (n > stack->size) return result_err(stack_check, "stack not big enough for this check");
  for (int i = 0; i < n; i++) {
    match(stack_ast_node_top_offset(stack, i), stack_ast_node_top, {
      check c_ = c[n - i - 1];
      if (c_.checktype == 1 && result_.ok.type != AST_TOKEN) return result_err(stack_check, "checktype TOKEN mismatch with AST_TOKEN");
      if (c_.checktype == 1 && result_.ok.token.t.type != c_.tokentype) return result_err(stack_check, "token doesn't match with check type");
      if (c_.checktype == 2 && result_.ok.type != c_.astnodetype) return result_err(stack_check, "checktype AST mismatch with AST_TOKEN");
      result.nodes[result.count++] = result_.ok;
    }, { return result_err(stack_check, "..."); });
  }
  return result_ok(stack_check, result);
}

#define res_top_off stack_ast_node_top
result_parser_run parser_run(tokenizer* t) {
  parser_ctx ctx = (parser_ctx){0};
  ctx.ast_stack = stack_ast_node_create();

  for (int i = 0; i < t->tokens.size; i++) {
    // 1. Put new token on stack
    token tok = t->tokens.buffer[i];
    stack_ast_node_push(&ctx.ast_stack, (ast_node){.type = AST_TOKEN, .token = {.t = tok }});
    for (int i = 0; i < ctx.ast_stack.size; i++) {
      match(stack_ast_node_top_offset(&ctx.ast_stack, i), stack_ast_node_top, {
        printf("%d\n", result_.ok.type);
      }, {});
    }


    // Just for testing
    match(stack_check(&ctx.ast_stack, check_seq({token(ID)})), stack_check, {
      printf("Test OK\n");
    }, continue;);

    // 2. Try to reduce the stack with this new token
    // vardec := <id> <eq> <id>
    match(stack_check(&ctx.ast_stack, check_seq({token(ID), token(EQ), token(ID)})), stack_check, {
      printf("OK\n");
    }, continue;);
  }

  stack_ast_node_free(&ctx.ast_stack);
  return result_err(parser_run, "Parser unimplemented");
}
