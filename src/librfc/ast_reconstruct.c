#include "ast_reconstruct.h"
#include <assert.h>
#include <stdio.h>
#define INDENT_ARGS INDENT_ARGSN(0)
#define TOKEN_ARGS(sv) (int) sv.len, sv.start

void ast_var_reconstruct(const variant_ast_vardecl* v) {
  const variant_ast_vardecl v2 = *v;
  match_variant(v2, ast_vardecl, {
    variant_case(ast_vardecl, Main, {
      printf("%.*s: %.*s", TOKEN_ARGS(v.Main.id), TOKEN_ARGS(v.Main.type));
    })
  })
}

void ast_lit_reconstruct(const variant_ast_lit* v) {
  const variant_ast_lit v2 = *v;
  match_variant(v2, ast_lit, {
    variant_case(ast_lit, Int, { printf("%.*s", TOKEN_ARGS(v.Int.v)); }) // TODO: This is printed as a string right now, this should not be permanent
    variant_case(ast_lit, Id, { printf("%.*s", TOKEN_ARGS(v.Int.v)); })
    variant_case(ast_lit, Double, { printf("%.*s", TOKEN_ARGS(v.Int.v)); })
  });
}
void ast_expr_reconstruct(const variant_ast_expr* v) {
  const variant_ast_expr v2 = *v;
  match_variant(v2, ast_expr, {
    variant_case(ast_expr, Disj, {
      ast_log_disj_reconstruct(v2.Disj.disj);
    })
  })
}
void ast_log_disj_reconstruct(const variant_ast_log_disj* v) {
  const variant_ast_log_disj v2 = *v;
  match_variant(v2, ast_log_disj, {
    variant_case(ast_log_disj, DisjConj, {
      ast_log_disj_reconstruct(v2.DisjConj.disj);
      printf("||");
      ast_log_conj_reconstruct(v2.DisjConj.conj);
    })
    variant_case(ast_log_disj, Conj, {
      ast_log_conj_reconstruct(v2.Conj.conj);
    })
  })
}
