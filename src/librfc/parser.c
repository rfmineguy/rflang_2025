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
      /* Try to reduce to literal
       *    literal := <intlit>
       */
      match(stack_check(&ctx.ast_stack, check_seq({token(INTLIT)})), stack_check, {
        stack_ast_node_pop_n(&ctx.ast_stack, 1);
        variant_ast_lit* lit = make_variant_alloc(ast_lit, arena_alloc);
        *lit = make_variant(ast_lit, Int, {.v = result_.ok.nodes[0].Token.t});
        stack_ast_node_push(&ctx.ast_stack, make_variant(ast_node, VariantLit, lit));
        printf("Reduced: (intlit)\n");
        number_reduced++;
      }, {});

      /* Try to reduce to literal
       *    literal := <id>
       */
      match(stack_check(&ctx.ast_stack, check_seq({token(ID)})), stack_check, {
        if (!lookahead || (lookahead && (lookahead->type == PLUS || lookahead->type == MINUS || lookahead->type == MUL || lookahead->type == DIV || lookahead->type == MOD))) {
          stack_ast_node_pop_n(&ctx.ast_stack, 1);
          variant_ast_lit* lit = make_variant_alloc(ast_lit, arena_alloc);

          // NOTE: This is incorrect. Do not make an Int variant, it should be an Id variant
          *lit = make_variant(ast_lit, Int, {.v = result_.ok.nodes[0].Token.t});
          stack_ast_node_push(&ctx.ast_stack, make_variant(ast_node, VariantLit, lit));
          printf("Reduced: (intlit)\n");
          number_reduced++;
        }
        else {}
      }, {});
    } while (number_reduced != 0);


    // TODO: Hello
  }
  // stack_ast_node_free(&ctx.ast_stack);
  return result_ok(parser_run, ctx);
  return result_err(parser_run, "Parser unimplemented");
}
