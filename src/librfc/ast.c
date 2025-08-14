#include "ast.h"
#include <stdio.h>
#define INDENT_FMT "%*.s"
#define INDENT_ARGS depth * 2, " "
#define TOKEN_ARGS(sv) (int) sv.len, sv.start

void ast_token_print(ast_token t, int depth) {
  printf(INDENT_FMT "Token(%s), '%.*s'\n", INDENT_ARGS, token_type_str(t.t.type), TOKEN_ARGS(t.t));
}

void ast_var_print(ast_vardecl v, int depth) {
  printf(INDENT_FMT "Var{type: %.*s, id: %.*s}\n", INDENT_ARGS, TOKEN_ARGS(v.type), TOKEN_ARGS(v.id));
}

void ast_lit_print(const variant_ast_lit* v, int depth) {
  const variant_ast_lit v2 = *v;
  match_variant(v2, ast_lit, {
    variant_case(ast_lit, Int,    { printf(INDENT_FMT "Lit{int: %.*s}\n", INDENT_ARGS, TOKEN_ARGS(v.Int)); })
    variant_case(ast_lit, Double, { printf(INDENT_FMT "Lit{dbl: %.*s}\n", INDENT_ARGS, TOKEN_ARGS(v.Int)); })
  })
}

void ast_node_print(variant_ast_node n, int depth) {
  printf("AST\n");
  match_variant(n, ast_node, {
    variant_case(ast_node, Token, { ast_token_print(n.Token, depth + 1); })
    variant_case(ast_node, Var, { ast_var_print(n.Var, depth + 1); })
    variant_default({
      printf(INDENT_FMT "Unknown ast_node variant\n", INDENT_ARGS);
    })
  })
}
