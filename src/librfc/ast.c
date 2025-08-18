#include "ast.h"
#include <stdio.h>
#define INDENT_FMT "%*.s"
#define INDENT_ARGSN(offset) (depth + offset) * 2, " "
#define INDENT_ARGS INDENT_ARGSN(0)
#define TOKEN_ARGS(sv) (int) sv.len, sv.start

void ast_token_print(ast_token t, int depth) {
  bool sameline = false;
  printf(INDENT_FMT "Token(%s), '%.*s'\n", INDENT_ARGS, token_type_str(t.t.type), TOKEN_ARGS(t.t));
}

void ast_var_print(variant_ast_vardecl* v, int depth, bool sameline) {
  const variant_ast_vardecl v2 = *v;
  if (!sameline) printf(INDENT_FMT, INDENT_ARGS);
  match_variant(v2, ast_vardecl, {
    variant_case(ast_vardecl, Main, { printf("Var{type: %.*s, id: %.*s}\n", TOKEN_ARGS(v.Main.type), TOKEN_ARGS(v.Main.id)); })
  })
}

void ast_lit_print(const variant_ast_lit* v, int depth, bool sameline) {
  const variant_ast_lit v2 = *v;
  if (!sameline) printf(INDENT_FMT, INDENT_ARGS);
  match_variant(v2, ast_lit, {
    variant_case(ast_lit, Int, { printf("Lit{int: %.*s}\n", TOKEN_ARGS(v.Int.v)); })
    variant_case(ast_lit, Id,  { printf("Lit{id: %.*s}\n", TOKEN_ARGS(v.Id.id)); })
    variant_case(ast_lit, Double, { printf("Lit{double: %.*s}\n", TOKEN_ARGS(v.Double.id)); })
  })
}

void ast_expr_print(const variant_ast_expr* v, int depth, bool sameline) {
  const variant_ast_expr v2 = *v;
  if (!sameline) printf(INDENT_FMT, INDENT_ARGS);
  match_variant(v2, ast_expr, {
    variant_case(ast_expr, Disj, {
        printf("Expr.");
        ast_log_disj_print(v2.Disj.disj, depth, true);
    })
  })
}

void ast_log_disj_print(const variant_ast_log_disj* v, int depth, bool sameline) {
  const variant_ast_log_disj v2 = *v;
  if (!sameline) printf(INDENT_FMT, INDENT_ARGS);
  match_variant(v2, ast_log_disj, {
    variant_case(ast_log_disj, DisjConj, {
      printf("Disj{\n");
      ast_log_disj_print(v2.DisjConj.disj, depth + 1, false);
      ast_log_conj_print(v2.DisjConj.conj, depth + 1, false);
      printf(INDENT_FMT "}\n", INDENT_ARGS);
    });
    variant_case(ast_log_disj, Conj, {
      printf("Disj.");
      ast_log_conj_print(v2.Conj.conj, depth, true);
    });
  })
}
void ast_log_conj_print(const variant_ast_log_conj* v, int depth, bool sameline) {
  const variant_ast_log_conj v2 = *v;
  if (!sameline) printf(INDENT_FMT, INDENT_ARGS);
  match_variant(v2, ast_log_conj, {
    variant_case(ast_log_conj, ConjRel, {
      printf("Conj {\n");
      ast_log_conj_print(v2.ConjRel.conj, depth + 1, false);
      ast_rel_print(v2.ConjRel.rel, depth + 1, false);
      printf(INDENT_FMT "}\n", INDENT_ARGS);
    })
    variant_case(ast_log_conj, Rel, {
      printf("Conj.");
      ast_rel_print(v2.Rel.conj, depth, true);
    })
  })
}
void ast_rel_print(const variant_ast_rel* v, int depth, bool sameline) {
  const variant_ast_rel v2 = *v;
  if (!sameline) printf(INDENT_FMT, INDENT_ARGS);
  match_variant(v2, ast_rel, {
    variant_case(ast_rel, RelME, {
      printf("Rel {\n");
      printf(INDENT_FMT "Operator: %.*s\n", INDENT_ARGSN(1), TOKEN_ARGS(v2.RelME.operator));
      ast_rel_print(v2.RelME.rel, depth + 1, false);
      ast_math_expr_print(v2.RelME.math_expr, depth + 1, false);
      printf(INDENT_FMT "}\n", INDENT_ARGS);
    });
    variant_case(ast_rel, MathExpr, {
      printf("Rel.");
      ast_math_expr_print(v2.MathExpr.conj, depth, true);
    })
  })
}
void ast_math_expr_print(const variant_ast_math_expr* v, int depth, bool sameline) {
  const variant_ast_math_expr v2 = *v;
  if (!sameline) printf(INDENT_FMT, INDENT_ARGS);
  match_variant(v2, ast_math_expr, {
    variant_case(ast_math_expr, METerm, {
      printf("MathExpr {\n");
      printf(INDENT_FMT "Operator: %.*s\n", INDENT_ARGSN(1), TOKEN_ARGS(v2.METerm.operator));
      ast_math_expr_print(v2.METerm.math_expr, depth + 1, false);
      ast_term_print(v2.METerm.term, depth + 1, false);
      printf(INDENT_FMT "}\n", INDENT_ARGS);
    })
    variant_case(ast_math_expr, Term, {
      printf("MathExpr.");
      ast_term_print(v2.Term.term, depth, true);
    })
  })
}
void ast_term_print(const variant_ast_term* v, int depth, bool sameline) {
  const variant_ast_term v2 = *v;
  if (!sameline) printf(INDENT_FMT, INDENT_ARGS);
  match_variant(v2, ast_term, {
    variant_case(ast_term, TermOpFactor, {
      printf("Term {\n");
      printf(INDENT_FMT "Operator: %.*s\n", INDENT_ARGSN(1), TOKEN_ARGS(v2.TermOpFactor.operator));
      ast_term_print(v2.TermOpFactor.term, depth + 1, false);
      ast_factor_print(v2.TermOpFactor.factor, depth + 1, false);
      printf(INDENT_FMT "}\n", INDENT_ARGS);
    })
    variant_case(ast_term, TermFactor, {
      printf("Term.");
      ast_factor_print(v2.TermFactor.factor, depth, true);
    })
  })
}
void ast_factor_print(const variant_ast_factor* v, int depth, bool sameline) {
  const variant_ast_factor v2 = (*v);
  if (!sameline) printf(INDENT_FMT, INDENT_ARGS);
  match_variant(v2, ast_factor, {
    variant_case(ast_factor, ExprParen, { 
      printf("Factor {\n");
      ast_expr_print(v2.ExprParen.expr, depth + 1, false);
      printf(INDENT_FMT "}\n", INDENT_ARGS);
    })
    variant_case(ast_factor, Lit, {
      printf("Factor.");
      ast_lit_print(v2.Lit.lit, depth, true);
    })
  })
}

void ast_node_print(variant_ast_node n, int depth) {
  match_variant(n, ast_node, {
    variant_case(ast_node, Token,           { ast_token_print(n.Token, depth); })
    variant_case(ast_node, VariantVar,      { ast_var_print(n.VariantVar, depth, false); })
    variant_case(ast_node, VariantLit,      { ast_lit_print(n.VariantLit, depth, false); })
    variant_case(ast_node, VariantLogDisj,  { ast_log_disj_print(n.VariantLogDisj, depth, false); })
    variant_case(ast_node, VariantLogConj,  { ast_log_conj_print(n.VariantLogConj, depth, false); })
    variant_case(ast_node, VariantRelation, { ast_rel_print(n.VariantRelation, depth, false); })
    variant_case(ast_node, VariantMathExpr, { ast_math_expr_print(n.VariantMathExpr, depth, false); })
    variant_case(ast_node, VariantTerm,     { ast_term_print(n.VariantTerm, depth, false); })
    variant_case(ast_node, VariantFactor,   { ast_factor_print(n.VariantFactor, depth, false); })
    variant_case(ast_node, VariantExpr,     { ast_expr_print(n.VariantExpr, depth, false); })
  })
}
