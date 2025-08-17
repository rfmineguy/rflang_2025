#include "parser.h"
#include "../thirdparty/arena.h"
#include <stdio.h>
#include "variant.h"

// see samples/main.g for the grammar

result_stack_check stack_check(const stack_ast_node* stack, check c[], int n) {
  variant_ast_node ns;
  check_result result = {0};
  if (n > stack->size) return result_err(stack_check, "stack not big enough for this check");
  for (int i = 0; i < n; i++) {
    match(stack_ast_node_top_offset(stack, i), stack_ast_node_top, {
      check c_ = c[n - i - 1];
      if (c_.checktype == 1) {
        match_variant(result_.ok, ast_node, {
          variant_case(ast_node, Token, {
            if (result_.ok.Token.t.type != c_.tokentype) return result_err(stack_check, "token doesn't match with check type");
          })
          variant_default({ return result_err(stack_check, "Checking for ast_token, got ast_node"); })
        })
      }
      if (c_.checktype == 2 && result_.ok.type != c_.astnodetype) {
        return result_err(stack_check, "Checking for ast_node, got ast_token");
      }
      result.nodes[result.count++] = result_.ok;
    }, { return result_err(stack_check, result_.err); });
  }
  return result_ok(stack_check, result);
}

#define res_top_off stack_ast_node_top
result_parser_run parser_run(tokenizer* t) {
#define arena_alloc(size) arena_alloc(&ctx.ast_arena, size)
  parser_ctx ctx = (parser_ctx){0};
  ctx.ast_stack = stack_ast_node_create();

  for (int i = 0; i < t->tokens.size; i++) {
    // 1. Put new token on stack
    token tok = t->tokens.buffer[i];
    token* lookahead = i < t->tokens.size - 1 ? &t->tokens.buffer[i+1] : NULL;
    stack_ast_node_push(&ctx.ast_stack, make_variant(ast_node, Token, tok));

    int number_reduced = 0;
    do {
      number_reduced = 0;
    } while (number_reduced != 0);


    // TODO: Hello
  }
  // stack_ast_node_free(&ctx.ast_stack);
  return result_ok(parser_run, ctx);
  return result_err(parser_run, "Parser unimplemented");
}
