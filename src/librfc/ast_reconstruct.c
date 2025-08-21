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
