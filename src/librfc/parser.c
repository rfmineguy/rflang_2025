#include "parser.h"
#include "token.h"
#include "../thirdparty/arena.h"
#include <stdio.h>
#include <string.h>
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

#define tok_list(...) (token_type[]){__VA_ARGS__}, sizeof((token_type[]){__VA_ARGS__})/sizeof(token_type)

bool lookahead_type_check(token* lookahead_token, token_type tokentypes[], int size) {
  if (!lookahead_token) return false;
  for (int i = 0; i < size; i++) {
    if (lookahead_token->type == tokentypes[i]) return true; 
  }
  return false;
}

#define res_top_off stack_ast_node_top
result_parser_run parser_run(tokenizer* t) {
#define arena_alloc_(size) arena_alloc(&ctx.ast_arena, size)
  parser_ctx ctx = (parser_ctx){0};
  ctx.ast_stack = stack_ast_node_create();

  for (int i = 0; i < t->tokens.size; i++) {
    // 1. Put new token on stack
    token tok = t->tokens.buffer[i];
    token* previous = i > 0 ? &t->tokens.buffer[i-1] : NULL;
    token* lookahead = i < t->tokens.size - 1 ? &t->tokens.buffer[i+1] : NULL;
    stack_ast_node_push(&ctx.ast_stack, make_variant(ast_node, Token, tok));

    int number_reduced = 0;
    do {
      number_reduced = 0;
      /* Try to reduce to vardecl
       *    vardecl := <id> <colon> <id>
       */
      match(stack_check(&ctx.ast_stack, check_seq({token(ID), token(COLON), ast(variant_ast_node_type_VariantType)})), stack_check, {
        stack_ast_node_pop_n(&ctx.ast_stack, 3);
        variant_ast_vardecl* vardecl_ = make_variant_alloc(ast_vardecl, arena_alloc_);
        *vardecl_ = make_variant(ast_vardecl, Main, ((vardecl){.id = result_.ok.nodes[2].Token.t, .type = result_.ok.nodes[0].VariantType}));

        stack_ast_node_push(&ctx.ast_stack, make_variant(ast_node, VariantVar, vardecl_));
        number_reduced++;
      },{})

      /* Try to reduce to type
       * "[" <type3> ";" <expression> "]"
       * "[" <type3> "]"
       * "*" <type3>
       */
      match(stack_check(&ctx.ast_stack, check_seq({token(MUL), ast(variant_ast_node_type_VariantType)})), stack_check, {
        stack_ast_node_pop_n(&ctx.ast_stack, 2);

        variant_ast_type* type = make_variant_alloc(ast_type, arena_alloc_);
        *type = make_variant(ast_type, Ptr, {.type = result_.ok.nodes[0].VariantType});
        stack_ast_node_push(&ctx.ast_stack, make_variant(ast_node, VariantType, type));
        number_reduced++;
      }, {});

      match(stack_check(&ctx.ast_stack, check_seq({token(LSBRK), ast(variant_ast_node_type_VariantType), token(RSBRK)})), stack_check, {
        stack_ast_node_pop_n(&ctx.ast_stack, 3);

        variant_ast_type* type = make_variant_alloc(ast_type, arena_alloc_);
        *type = make_variant(ast_type, Array, ((type_array){.type = result_.ok.nodes[1].VariantType, .expr_opt = 0}));
        stack_ast_node_push(&ctx.ast_stack, make_variant(ast_node, VariantType, type));
        number_reduced++;
      }, {})

      match(stack_check(&ctx.ast_stack, check_seq({token(LSBRK), ast(variant_ast_node_type_VariantType), token(SEMI), ast(variant_ast_node_type_VariantExpr), token(RSBRK)})), stack_check, {
        stack_ast_node_pop_n(&ctx.ast_stack, 5);

        variant_ast_type* type = make_variant_alloc(ast_type, arena_alloc_);
        *type = make_variant(ast_type, Array, ((type_array){.type = result_.ok.nodes[3].VariantType, .expr_opt = result_.ok.nodes[1].VariantExpr}));
        stack_ast_node_push(&ctx.ast_stack, make_variant(ast_node, VariantType, type));
        number_reduced++;
      }, {})

      match(stack_check(&ctx.ast_stack, check_seq({token(LSBRK), ast(variant_ast_node_type_VariantType), token(RSBRK)})), stack_check, {
        stack_ast_node_pop_n(&ctx.ast_stack, 2);

        variant_ast_type* type = make_variant_alloc(ast_type, arena_alloc_);
        *type = make_variant(ast_type, Id, {.id = result_.ok.nodes[0].VariantType->Id.id});
        stack_ast_node_push(&ctx.ast_stack, make_variant(ast_node, VariantType, type));
        number_reduced++;
      }, {});

      /* Try to reduce id
       *    literal := <id>
       *    type3   := <id>       if the lookahead is not an operator or colon
       */
      match(stack_check(&ctx.ast_stack, check_seq({token(ID)})), stack_check, {
        if (lookahead && (lookahead->type == LPAR)) {
          continue; // expect function call
        }
        else if (lookahead && (!token_is_operator(*lookahead) && lookahead->type != COLON)) {
          // expect type
          stack_ast_node_pop_n(&ctx.ast_stack, 1);
          variant_ast_type* type = make_variant_alloc(ast_type, arena_alloc_);
          *type = make_variant(ast_type, Id, {.id = result_.ok.nodes[0].Token.t});
          stack_ast_node_push(&ctx.ast_stack, make_variant(ast_node, VariantType, type));
          number_reduced++;
        }
        else if (lookahead && (lookahead->type != COLON)) {
          stack_ast_node_pop_n(&ctx.ast_stack, 1);
          variant_ast_lit* lit = make_variant_alloc(ast_lit, arena_alloc_);

          *lit = make_variant(ast_lit, Id, {.id = result_.ok.nodes[0].Token.t});
          stack_ast_node_push(&ctx.ast_stack, make_variant(ast_node, VariantLit, lit));
          number_reduced++;
          printf("Reduced id to literal\n");
        }
      }, {});

      /* Try to reduce to literal
       *    literal := <intlit>
       */
      match(stack_check(&ctx.ast_stack, check_seq({token(INTLIT)})), stack_check, {
        stack_ast_node_pop_n(&ctx.ast_stack, 1);
        variant_ast_lit* lit = make_variant_alloc(ast_lit, arena_alloc_);
        *lit = make_variant(ast_lit, Int, {.v = result_.ok.nodes[0].Token.t});
        stack_ast_node_push(&ctx.ast_stack, make_variant(ast_node, VariantLit, lit));
        number_reduced++;
      }, {});

      /* Try to reduce a integer literal in the form
       *    factor := <literal>
       */
      match(stack_check(&ctx.ast_stack, check_seq({ast(variant_ast_node_type_VariantLit)})), stack_check, {
        if (previous && (previous->type == KEYWORD && strncmp("func", previous->start, previous->len) == 0)) continue;
        stack_ast_node_pop_n(&ctx.ast_stack, 1);
        variant_ast_factor* factor = make_variant_alloc(ast_factor, arena_alloc_);
        *factor = make_variant(ast_factor, Lit, {.lit = result_.ok.nodes[0].VariantLit});

        stack_ast_node_push(&ctx.ast_stack, make_variant(ast_node, VariantFactor, factor));
        number_reduced++;
      }, {});

      /* Try to reduce an expression to factor
       *    factor := '(' <expression> ')'
       */
      match(stack_check(&ctx.ast_stack, check_seq({token(LPAR), ast(variant_ast_node_type_VariantExpr), token(RPAR)})), stack_check, {
        stack_ast_node_pop_n(&ctx.ast_stack, 3);
        variant_ast_factor* factor = make_variant_alloc(ast_factor, arena_alloc_);
        *factor = make_variant(ast_factor, ExprParen, (factor_expr_paren){.expr = result_.ok.nodes[1].VariantExpr});

        stack_ast_node_push(&ctx.ast_stack, make_variant(ast_node, VariantFactor, factor));
        number_reduced++;
      }, {});

      /* Try to reduce a term
       *  term := <term> * <factor>
       *        | <term> / <factor>
       *        | <term> % <factor>
       */
      match(stack_check(&ctx.ast_stack, check_seq({ast(variant_ast_node_type_VariantTerm), token(MUL), ast(variant_ast_node_type_VariantFactor)})), stack_check, {
        stack_ast_node_pop_n(&ctx.ast_stack, 3);
        variant_ast_term* term = make_variant_alloc(ast_term, arena_alloc_);
        *term = make_variant(ast_term, TermOpFactor, ((term_op_factor){.term = result_.ok.nodes[2].VariantTerm, .factor = result_.ok.nodes[0].VariantFactor, .operator = result_.ok.nodes[1].Token.t}));
        stack_ast_node_push(&ctx.ast_stack, make_variant(ast_node, VariantTerm, term));
        number_reduced++;
      }, {})
      match(stack_check(&ctx.ast_stack, check_seq({ast(variant_ast_node_type_VariantTerm), token(DIV), ast(variant_ast_node_type_VariantFactor)})), stack_check, {
        stack_ast_node_pop_n(&ctx.ast_stack, 3);
        variant_ast_term* term = make_variant_alloc(ast_term, arena_alloc_);
        *term = make_variant(ast_term, TermOpFactor, ((term_op_factor){.term = result_.ok.nodes[2].VariantTerm, .factor = result_.ok.nodes[0].VariantFactor, .operator = result_.ok.nodes[1].Token.t}));
        stack_ast_node_push(&ctx.ast_stack, make_variant(ast_node, VariantTerm, term));
        number_reduced++;
      }, {})
      match(stack_check(&ctx.ast_stack, check_seq({ast(variant_ast_node_type_VariantTerm), token(MOD), ast(variant_ast_node_type_VariantFactor)})), stack_check, {
        stack_ast_node_pop_n(&ctx.ast_stack, 3);
        variant_ast_term* term = make_variant_alloc(ast_term, arena_alloc_);
        *term = make_variant(ast_term, TermOpFactor, ((term_op_factor){.term = result_.ok.nodes[2].VariantTerm, .factor = result_.ok.nodes[0].VariantFactor, .operator = result_.ok.nodes[1].Token.t}));
        stack_ast_node_push(&ctx.ast_stack, make_variant(ast_node, VariantTerm, term));
        number_reduced++;
      }, {})

      /* Try to reduce a factor into a term IF the factor has '*', '/', or '%' after
       *  <term> := <factor>
       */
      match(stack_check(&ctx.ast_stack, check_seq({ast(variant_ast_node_type_VariantFactor)})), stack_check, {
        stack_ast_node_pop_n(&ctx.ast_stack, 1);
        variant_ast_term* term = make_variant_alloc(ast_term, arena_alloc_);
        *term = make_variant(ast_term, TermFactor, result_.ok.nodes[0].VariantFactor);

        stack_ast_node_push(&ctx.ast_stack, make_variant(ast_node, VariantTerm, term));
        number_reduced++;
      }, {})

      /* Try to reduce a math_expression
       *  math_expr := <math_expr> + <term>
       *               <math_expr> - <term>
       * Note: If the lookahead is any operator that has higher precedence than '+' and '-' we ignore this reductio
       */
      match(stack_check(&ctx.ast_stack, check_seq({ast(variant_ast_node_type_VariantMathExpr), token(PLUS), ast(variant_ast_node_type_VariantTerm)})), stack_check, {
        if (lookahead_type_check(lookahead, tok_list(GTEQ, LTEQ, GT, LT, MUL, DIV, MOD))) continue;
        stack_ast_node_pop_n(&ctx.ast_stack, 3);
        variant_ast_math_expr* mathexpr = make_variant_alloc(ast_math_expr, arena_alloc_);
        *mathexpr = make_variant(ast_math_expr, METerm, ((math_expr_me_term){.math_expr = result_.ok.nodes[2].VariantMathExpr, .term = result_.ok.nodes[0].VariantTerm, .operator = result_.ok.nodes[1].Token.t}));
        stack_ast_node_push(&ctx.ast_stack, make_variant(ast_node, VariantMathExpr, mathexpr));
        number_reduced++;
      }, {});

      match(stack_check(&ctx.ast_stack, check_seq({ast(variant_ast_node_type_VariantMathExpr), token(MINUS), ast(variant_ast_node_type_VariantTerm)})), stack_check, {
        if (lookahead_type_check(lookahead, tok_list(GTEQ, LTEQ, GT, LT, PLUS, MINUS, MUL, DIV, MOD, EOF_))) continue;
        stack_ast_node_pop_n(&ctx.ast_stack, 3);
        variant_ast_math_expr* mathexpr = make_variant_alloc(ast_math_expr, arena_alloc_);
        *mathexpr = make_variant(ast_math_expr, METerm, ((math_expr_me_term){.math_expr = result_.ok.nodes[2].VariantMathExpr, .term = result_.ok.nodes[0].VariantTerm, .operator = result_.ok.nodes[1].Token.t}));
        stack_ast_node_push(&ctx.ast_stack, make_variant(ast_node, VariantMathExpr, mathexpr));
        number_reduced++;
      }, {});

      /* Try to reduce a term into math_expression IF the term has '+', or '-' after
       *  math_expr := <term>
       */
      match(stack_check(&ctx.ast_stack, check_seq({ast(variant_ast_node_type_VariantTerm)})), stack_check, {
        if (!lookahead_type_check(lookahead, tok_list(GTEQ, LTEQ, GT, LT, PLUS, MINUS, RPAR, EOF_, KEYWORD, RCBRK, LCBRK, RSBRK))) continue;
        stack_ast_node_pop_n(&ctx.ast_stack, 1);
        variant_ast_math_expr* mathexpr = make_variant_alloc(ast_math_expr, arena_alloc_);
        *mathexpr = make_variant(ast_math_expr, Term, ((math_expr_term){.term = result_.ok.nodes[0].VariantTerm}));
        stack_ast_node_push(&ctx.ast_stack, make_variant(ast_node, VariantMathExpr, mathexpr));
        number_reduced++;
      }, {})
      
      /* Try to reduce a math_expression to a relational
       *  relation := <relational> '>=' <math_expr>
       *  relation := <relational> '<=' <math_expr>
       *  relation := <relational> '>' <math_expr>
       *  relation := <relational> '<' <math_expr>
       *  relation := <relational> '==' <math_expr>
       */
      match(stack_check(&ctx.ast_stack, check_seq({ast(variant_ast_node_type_VariantRelation), token(GTEQ), ast(variant_ast_node_type_VariantMathExpr)})), stack_check, {
        if (!lookahead_type_check(lookahead, tok_list(DAND, DOR, EOF_, KEYWORD, RCBRK, LCBRK, RSBRK))) continue;
        // if (lookahead && !(lookahead == DAND || lookahead->type == DOR || lookahead->type == EOF_)) continue;
        stack_ast_node_pop_n(&ctx.ast_stack, 3);
        variant_ast_rel* rel = make_variant_alloc(ast_rel, arena_alloc_);
        *rel = make_variant(ast_rel, RelME, ((rel_rel_math_expr){.rel = result_.ok.nodes[2].VariantRelation, .operator = result_.ok.nodes[1].Token.t, .math_expr = result_.ok.nodes[0].VariantMathExpr}));
        stack_ast_node_push(&ctx.ast_stack, make_variant(ast_node, VariantRelation, rel));
        number_reduced++;
      }, {})

      match(stack_check(&ctx.ast_stack, check_seq({ast(variant_ast_node_type_VariantRelation), token(LTEQ), ast(variant_ast_node_type_VariantMathExpr)})), stack_check, {
        if (!lookahead_type_check(lookahead, tok_list(DAND, DOR, EOF_, KEYWORD, RCBRK, LCBRK, RSBRK))) continue;
        stack_ast_node_pop_n(&ctx.ast_stack, 3);
        variant_ast_rel* rel = make_variant_alloc(ast_rel, arena_alloc_);
        *rel = make_variant(ast_rel, RelME, ((rel_rel_math_expr){.rel = result_.ok.nodes[2].VariantRelation, .operator = result_.ok.nodes[1].Token.t, .math_expr = result_.ok.nodes[0].VariantMathExpr}));
        stack_ast_node_push(&ctx.ast_stack, make_variant(ast_node, VariantRelation, rel));
        number_reduced++;
      }, {})

      match(stack_check(&ctx.ast_stack, check_seq({ast(variant_ast_node_type_VariantRelation), token(GT), ast(variant_ast_node_type_VariantMathExpr)})), stack_check, {
        if (!lookahead_type_check(lookahead, tok_list(DAND, DOR, EOF_, KEYWORD, RCBRK, LCBRK, RSBRK))) continue;
        stack_ast_node_pop_n(&ctx.ast_stack, 3);
        variant_ast_rel* rel = make_variant_alloc(ast_rel, arena_alloc_);
        *rel = make_variant(ast_rel, RelME, ((rel_rel_math_expr){.rel = result_.ok.nodes[2].VariantRelation, .operator = result_.ok.nodes[1].Token.t, .math_expr = result_.ok.nodes[0].VariantMathExpr}));
        stack_ast_node_push(&ctx.ast_stack, make_variant(ast_node, VariantRelation, rel));
        number_reduced++;
      }, {})

      match(stack_check(&ctx.ast_stack, check_seq({ast(variant_ast_node_type_VariantRelation), token(LT), ast(variant_ast_node_type_VariantMathExpr)})), stack_check, {
        if (!lookahead_type_check(lookahead, tok_list(DAND, DOR, EOF_, KEYWORD, RCBRK, LCBRK, RSBRK))) continue;
        stack_ast_node_pop_n(&ctx.ast_stack, 3);
        variant_ast_rel* rel = make_variant_alloc(ast_rel, arena_alloc_);
        *rel = make_variant(ast_rel, RelME, ((rel_rel_math_expr){.rel = result_.ok.nodes[2].VariantRelation, .operator = result_.ok.nodes[1].Token.t, .math_expr = result_.ok.nodes[0].VariantMathExpr}));
        stack_ast_node_push(&ctx.ast_stack, make_variant(ast_node, VariantRelation, rel));
        number_reduced++;
      }, {})

      match(stack_check(&ctx.ast_stack, check_seq({ast(variant_ast_node_type_VariantRelation), token(DEQ), ast(variant_ast_node_type_VariantMathExpr)})), stack_check, {
        if (!lookahead_type_check(lookahead, tok_list(DAND, DOR, EOF_, KEYWORD, RCBRK, LCBRK, RSBRK))) continue;
        stack_ast_node_pop_n(&ctx.ast_stack, 3);
        variant_ast_rel* rel = make_variant_alloc(ast_rel, arena_alloc_);
        *rel = make_variant(ast_rel, RelME, ((rel_rel_math_expr){.rel = result_.ok.nodes[2].VariantRelation, .operator = result_.ok.nodes[1].Token.t, .math_expr = result_.ok.nodes[0].VariantMathExpr}));
        stack_ast_node_push(&ctx.ast_stack, make_variant(ast_node, VariantRelation, rel));
        number_reduced++;
      }, {})

      /* Try to reduce a math_expression to a relational
       */
      match(stack_check(&ctx.ast_stack, check_seq({ast(variant_ast_node_type_VariantMathExpr)})), stack_check, {
        if (lookahead_type_check(lookahead, tok_list(PLUS, MINUS))) continue;
        stack_ast_node_pop_n(&ctx.ast_stack, 1);
        variant_ast_rel* rel = make_variant_alloc(ast_rel, arena_alloc_);
        *rel = make_variant(ast_rel, MathExpr, ((rel_math_expr) {.conj = result_.ok.nodes[0].VariantMathExpr}));
        stack_ast_node_push(&ctx.ast_stack, make_variant(ast_node, VariantRelation, rel));
        number_reduced++;
      },{})

      /* Try to reduce a relational to a logical conj
       */
      match(stack_check(&ctx.ast_stack, check_seq({ast(variant_ast_node_type_VariantRelation)})), stack_check, {
        if (lookahead_type_check(lookahead, tok_list(DAND, GTEQ, LTEQ, GT, LT))) continue;
        stack_ast_node_pop_n(&ctx.ast_stack, 1);
        variant_ast_log_conj* conj = make_variant_alloc(ast_log_conj, arena_alloc_);
        *conj = make_variant(ast_log_conj, Rel, ((log_conj_rel) {.conj = result_.ok.nodes[0].VariantRelation}));
        stack_ast_node_push(&ctx.ast_stack, make_variant(ast_node, VariantLogConj, conj));
        number_reduced++;
      },{})

      /* Try to reduce a logical conj to a logical disj
       */
      match(stack_check(&ctx.ast_stack, check_seq({ast(variant_ast_node_type_VariantLogConj)})), stack_check, {
        if (lookahead_type_check(lookahead, tok_list(DOR))) continue;
        stack_ast_node_pop_n(&ctx.ast_stack, 1);
        variant_ast_log_disj* disj = make_variant_alloc(ast_log_disj, arena_alloc_);
        *disj = make_variant(ast_log_disj, Conj, ((log_disj_conj) {.conj = result_.ok.nodes[0].VariantLogConj}));
        stack_ast_node_push(&ctx.ast_stack, make_variant(ast_node, VariantLogDisj, disj));
        number_reduced++;
      },{})

      /* Try to reduce a logical disj to an expression
       */
      match(stack_check(&ctx.ast_stack, check_seq({ast(variant_ast_node_type_VariantLogDisj)})), stack_check, {
        stack_ast_node_pop_n(&ctx.ast_stack, 1);
        variant_ast_expr* expr = make_variant_alloc(ast_expr, arena_alloc_);
        *expr = make_variant(ast_expr, Disj, ((expr_log_disj) {.disj = result_.ok.nodes[0].VariantLogDisj}));
        stack_ast_node_push(&ctx.ast_stack, make_variant(ast_node, VariantExpr, expr));
        number_reduced++;
      },{})
    } while (number_reduced != 0);

    // TODO: Hello
  }
  // stack_ast_node_free(&ctx.ast_stack);
  return result_ok(parser_run, ctx);
  return result_err(parser_run, "Parser unimplemented");
}
