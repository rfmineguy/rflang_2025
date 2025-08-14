#include "ast.h"
#include <stdio.h>
#define INDENT_FMT "%*.s"
#define INDENT_ARGSN(offset) (depth + offset) * 2, " "
#define INDENT_ARGS INDENT_ARGSN(0)
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
      ast_rel_print(v2.ConjRel.rel, depth + 1);
    })
    variant_case(ast_log_conj, Rel, {
      ast_rel_print(v2.Rel.conj, depth + 1);
    })
  })
  printf(INDENT_FMT "}\n", INDENT_ARGS);
}
void ast_rel_print(const variant_ast_rel* v, int depth) {
  printf(INDENT_FMT "MathExpr {\n", INDENT_ARGS);
  const variant_ast_rel v2 = *v;
  match_variant(v2, ast_rel, {
    variant_case(ast_relation, RelME, {
      printf(INDENT_FMT "Operator: %.*s\n", INDENT_ARGSN(1), TOKEN_ARGS(v2.RelME.operator));
      ast_rel_print(v2.RelME.rel, depth + 1);
      ast_math_expr_print(v2.RelME.math_expr, depth + 1);
    });
    variant_case(ast_relation, ME, {
      ast_math_expr_print(v2.ME.conj, depth + 1);
    })
  })
  printf(INDENT_FMT "}\n", INDENT_ARGS);
}
void ast_math_expr_print(const variant_ast_math_expr* v, int depth) {
  printf(INDENT_FMT "MathExpr {\n", INDENT_ARGS);
  const variant_ast_math_expr v2 = *v;
  match_variant(v2, ast_math_expr, {
    variant_case(ast_math_expr, METerm, {
      printf(INDENT_FMT "Operator: %.*s\n", INDENT_ARGSN(1), TOKEN_ARGS(v2.METerm.operator));
      ast_math_expr_print(v2.METerm.math_expr, depth + 1);
      ast_term_print(v2.METerm.term, depth + 1);
    })
    variant_case(ast_math_expr, Term, {
      ast_term_print(v2.Term.term, depth + 1);
    })
  })
  printf(INDENT_FMT "}\n", INDENT_ARGS);
}
void ast_term_print(const variant_ast_term* v, int depth) {
  printf(INDENT_FMT "Term {\n", INDENT_ARGS);
  const variant_ast_term v2 = *v;
  match_variant(v2, ast_term, {
    variant_case(ast_term, TermFactor, {
      ast_factor_print(v2.TermFactor.factor, depth + 1);
    })
    variant_case(ast_term, TermOpFactor, {
      printf(INDENT_FMT "Operator: %.*s\n", INDENT_ARGSN(1), TOKEN_ARGS(v2.TermOpFactor.operator));
      ast_term_print(v2.TermOpFactor.term, depth + 1);
      ast_factor_print(v2.TermOpFactor.factor, depth + 1);
    })
  })
  printf(INDENT_FMT "}\n", INDENT_ARGS);
}
void ast_factor_print(const variant_ast_factor* v, int depth) {
  printf(INDENT_FMT "Factor {\n", INDENT_ARGS);
  const variant_ast_factor v2 = (*v);
  match_variant(v2, ast_factor, {
    variant_case(ast_factor, ExprParen, { ast_expr_print(v2.ExprParen.expr, depth + 1); })
    variant_case(ast_factor, Lit, { ast_lit_print(v2.Lit.lit, depth + 1); })
  })
  printf(INDENT_FMT "}\n", INDENT_ARGS);
}

void ast_node_print(variant_ast_node n, int depth) {
  match_variant(n, ast_node, {
    variant_case(ast_node, Token,           { ast_token_print(n.Token, depth + 1); })
    variant_case(ast_node, Var,             { ast_var_print(n.Var, depth + 1); })
    variant_case(ast_node, VariantLit,      { ast_lit_print(n.VariantLit, depth + 1); })
    variant_case(ast_node, VariantLogDisj,  { ast_log_disj_print(n.VariantLogDisj, depth + 1); })
    variant_case(ast_node, VariantLogConj,  { ast_log_conj_print(n.VariantLogConj, depth + 1); })
    variant_case(ast_node, VariantRelation, { ast_rel_print(n.VariantRelation, depth + 1); })
    variant_case(ast_node, VariantMathExpr, { ast_math_expr_print(n.VariantMathExpr, depth + 1); })
    variant_case(ast_node, VariantTerm,     { ast_term_print(n.VariantTerm, depth + 1); })
    variant_case(ast_node, VariantFactor,   { ast_factor_print(n.VariantFactor, depth + 1); })
  })
}
