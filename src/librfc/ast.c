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
    variant_case(ast_lit, Int,    { printf(INDENT_FMT "Lit{int: %.*s}\n", INDENT_ARGS, TOKEN_ARGS(v.Int.v)); })
    variant_case(ast_lit, Double, { printf(INDENT_FMT "Lit{dbl: %.*s}\n", INDENT_ARGS, TOKEN_ARGS(v.Int.v)); })
  })
}

void ast_expr_print(const variant_ast_expr* v, int depth) {
  const variant_ast_expr v2 = *v;
  match_variant(v2, ast_expr, {
    variant_case(ast_expr, Disj, {
        printf(INDENT_FMT "Expr {\n", INDENT_ARGS);
        ast_log_conj_print(v2.Disj.conj, depth + 1);
        printf(INDENT_FMT "}\n", INDENT_ARGS);
    })
  })
}

void ast_log_disj_print(const variant_ast_log_disj* v, int depth) {
  printf(INDENT_FMT "Disj {\n", INDENT_ARGS);
  const variant_ast_log_disj v2 = *v;
  match_variant(v2, ast_log_disj, {
    variant_case(ast_log_disj, DisjConj, {
      ast_log_disj_print(v2.DisjConj.disj, depth + 1);
      ast_log_conj_print(v2.DisjConj.conj, depth + 1);
    });
    variant_case(ast_log_disj, Conj, {
      ast_log_conj_print(v2.Conj.conj, depth + 1);
    });
  })
  printf(INDENT_FMT "}\n", INDENT_ARGS);
}
void ast_log_conj_print(const variant_ast_log_conj* v, int depth) {
  printf(INDENT_FMT "Conj {\n", INDENT_ARGS);
  const variant_ast_log_conj v2 = *v;
  match_variant(v2, ast_log_conj, {
    variant_case(ast_log_conj, ConjRel, {
      ast_log_conj_print(v2.ConjRel.conj, depth + 1);
      ast_log_rel_print(v2.ConjRel.rel, depth + 1);
    })
    variant_case(ast_log_conj, Rel, {
      ast_log_rel_print(v2.Rel.conj, depth + 1);
    })
  })
  printf(INDENT_FMT "}\n", INDENT_ARGS);
}
void ast_log_rel_print(const variant_ast_rel* v, int depth) {
  const variant_ast_rel v2 = *v;
  match_variant(v2, ast_rel, {
    variant_case(ast_relation, RelME, {})
    variant_case(ast_relation, ME, {})
  })
}
void ast_node_print(variant_ast_node n, int depth) {
  match_variant(n, ast_node, {
    variant_case(ast_node, Token,       { ast_token_print(n.Token, depth + 1); })
    variant_case(ast_node, Var,         { ast_var_print(n.Var, depth + 1); })
    variant_case(ast_node, VariantLit,  { ast_lit_print(n.VariantLit, depth + 1); })
    variant_case(ast_node, VariantExpr, { ast_expr_print(n.VariantExpr, depth + 1); })
  })
}
