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
void ast_log_conj_reconstruct(const variant_ast_log_conj* v) {
  const variant_ast_log_conj v2 = *v;
  match_variant(v2, ast_log_conj, {
    variant_case(ast_log_conj, ConjRel, {
      ast_log_conj_reconstruct(v2.ConjRel.conj);
      printf("&&");
      ast_rel_reconstruct(v2.ConjRel.rel);
    })
    variant_case(ast_log_conj, Rel, {
      ast_rel_reconstruct(v2.Rel.conj);
    })
  })
}
void ast_rel_reconstruct(const variant_ast_rel* v) {
  const variant_ast_rel v2 = *v;
  match_variant(v2, ast_rel, {
    variant_case(ast_rel, RelME, {
      ast_rel_reconstruct(v2.RelME.rel);
      printf("%.*s", TOKEN_ARGS(v2.RelME.operator));
      ast_math_expr_reconstruct(v2.RelME.math_expr);
    })
    variant_case(ast_rel, MathExpr, {
      ast_math_expr_reconstruct(v2.MathExpr.conj);
    })
  })
}
void ast_math_expr_reconstruct(const variant_ast_math_expr* v) {
  const variant_ast_math_expr v2 = *v;
  match_variant(v2, ast_math_expr, {
    variant_case(ast_math_expr, METerm, {
      ast_math_expr_reconstruct(v2.METerm.math_expr);
      printf("%.*s", TOKEN_ARGS(v2.METerm.operator));
      ast_term_reconstruct(v2.METerm.term);
    })
    variant_case(ast_math_expr, Term, {
      ast_term_reconstruct(v2.Term.term);
    })
  })
}
void ast_term_reconstruct(const variant_ast_term* v) {
  const variant_ast_term v2 = *v;
  match_variant(v2, ast_term, {
    variant_case(ast_term, TermOpFactor, {
      ast_term_reconstruct(v.TermOpFactor.term);
      printf("%.*s", TOKEN_ARGS(v.TermOpFactor.operator));
      ast_factor_reconstruct(v.TermOpFactor.factor);
    })
    variant_case(ast_term, TermFactor, {
      ast_factor_reconstruct(v.TermFactor.factor);
    })
  })
}
void ast_factor_reconstruct(const variant_ast_factor* v) {
  const variant_ast_factor v2 = *v;
  match_variant(v2, ast_factor, {
    variant_case(ast_factor, ExprParen, {
      printf("(");
      ast_expr_reconstruct(v.ExprParen.expr);
      printf(")");
    })
    variant_case(ast_factor, Lit, {
      ast_lit_reconstruct(v.Lit.lit);
    })
  });
}
void ast_reconstruct(variant_ast_node n) {
  match_variant(n, ast_node, {
    variant_case(ast_node, Token,           { assert(0 && "Doesn't make sense to reconstruct token directly"); })
    variant_case(ast_node, VariantVar,      { ast_var_reconstruct(n.VariantVar); })
    variant_case(ast_node, VariantLit,      { ast_lit_reconstruct(n.VariantLit); })
    variant_case(ast_node, VariantLogDisj,  { ast_log_disj_reconstruct(n.VariantLogDisj); })
    variant_case(ast_node, VariantLogConj,  { ast_log_conj_reconstruct(n.VariantLogConj); })
    variant_case(ast_node, VariantRelation, { ast_rel_reconstruct(n.VariantRelation); })
    variant_case(ast_node, VariantMathExpr, { ast_math_expr_reconstruct(n.VariantMathExpr); })
    variant_case(ast_node, VariantTerm,     { ast_term_reconstruct(n.VariantTerm); })
    variant_case(ast_node, VariantFactor,   { ast_factor_reconstruct(n.VariantFactor); })
    variant_case(ast_node, VariantExpr,     { ast_expr_reconstruct(n.VariantExpr); })
  })
}
