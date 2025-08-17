#ifndef AST_H
#define AST_H

#include "token.h"
#include "variant.h"
#include <stdbool.h>
forward_dec_variant(ast_expr);
forward_dec_variant(ast_node);
forward_dec_variant(ast_lit);
forward_dec_variant(ast_log_disj);
forward_dec_variant(ast_log_conj);
forward_dec_variant(ast_rel);
forward_dec_variant(ast_math_expr);
forward_dec_variant(ast_term);
forward_dec_variant(ast_factor);
forward_dec_variant(ast_token);
forward_dec_variant(ast_vardecl);


/*
 * ==============
 * Variant setup
 * ==============
 */
#define ast_expr_variant(X)\
  X(Disj, ast_expr, expr_log_disj)\

#define ast_log_disj_variant(X)\
  X(DisjConj, ast_log_disj, log_disj_disj_conj)\
  X(Conj, ast_log_disj, log_disj_conj)

#define ast_log_conj_variant(X)\
  X(ConjRel, ast_log_conj, log_conj_conj_rel)\
  X(Rel, ast_log_conj, log_conj_rel)

#define ast_rel_variant(X)\
  X(RelME, ast_rel, rel_rel_math_expr)\
  X(MathExpr, ast_rel, rel_math_expr)

#define ast_math_expr_variant(X)\
  X(METerm, ast_math_expr, math_expr_me_term)\
  X(Term, ast_math_expr, math_expr_term)

#define ast_term_variant(X)\
  X(TermFactor, ast_term, term_factor)\
  X(TermOpFactor, ast_term,  term_op_factor)

#define ast_factor_variant(X)\
  X(ExprParen, ast_factor, factor_expr_paren)\
  X(Lit, ast_factor, factor_lit)

#define ast_lit_variant(X)\
  X(Int, ast_lit, lit_int)\
  X(Double, ast_lit, lit_double)\

#define ast_node_variant(X)\
  X(Token, ast_node, ast_token)\
  X(Var,   ast_node, ast_vardecl)\
  X(VariantLit,      ast_node, variant_ast_lit*)\
  X(VariantExpr,     ast_node, variant_ast_expr*)\
  X(VariantLogDisj,  ast_node, variant_ast_log_disj*)\
  X(VariantLogConj,  ast_node, variant_ast_log_conj*)\
  X(VariantRelation, ast_node, variant_ast_rel*)\
  X(VariantMathExpr, ast_node, variant_ast_math_expr*)\
  X(VariantTerm,     ast_node, variant_ast_term*)\
  X(VariantFactor,   ast_node, variant_ast_factor*)\
 
typedef struct { token t; } ast_token;
typedef struct { token id; token type; } ast_vardecl;

typedef struct { variant_ast_log_disj* disj; } expr_log_disj;

// Logical Disjunction (Logical Or)
typedef struct { variant_ast_log_disj* disj; variant_ast_log_conj* conj; } log_disj_disj_conj;
typedef struct { variant_ast_log_conj* conj; } log_disj_conj;

// Logical Conjunction (Logical And)
typedef struct { variant_ast_log_conj* conj; variant_ast_rel* rel; } log_conj_conj_rel;
typedef struct { variant_ast_rel* conj; } log_conj_rel;

// Relational
typedef struct { variant_ast_rel* rel; variant_ast_math_expr* math_expr; token operator; } rel_rel_math_expr;
typedef struct { variant_ast_math_expr* conj; } rel_math_expr;

// Math Expression
typedef struct { variant_ast_math_expr* math_expr; variant_ast_term* term; token operator; } math_expr_me_term;
typedef struct { variant_ast_term* term; } math_expr_term;

// Term
typedef struct { variant_ast_term* term; variant_ast_factor* factor; token operator; } term_op_factor;
typedef struct { variant_ast_factor* factor; } term_factor;

// Factor
typedef struct { variant_ast_expr* expr; } factor_expr_paren;
typedef struct { variant_ast_lit* lit; } factor_lit;

// Lit
typedef struct { token v; } lit_int;
typedef struct { token id; } lit_double;

define_variant(ast_expr, ast_expr_variant);
define_variant(ast_log_disj, ast_log_disj_variant)
define_variant(ast_log_conj, ast_log_conj_variant)
define_variant(ast_rel, ast_rel_variant)
define_variant(ast_math_expr, ast_math_expr_variant)
define_variant(ast_term, ast_term_variant)
define_variant(ast_factor, ast_factor_variant)
define_variant(ast_lit, ast_lit_variant)
define_variant(ast_node, ast_node_variant)

// define_variant(ast_expr, ast_expr_variant)
// define_variant(ast_lit,  ast_lit_variant)

void ast_node_print(variant_ast_node n, int depth);

void ast_lit_print(const variant_ast_lit* n, int depth, bool sameline);
void ast_expr_print(const variant_ast_expr* n, int depth, bool sameline);
void ast_log_disj_print(const variant_ast_log_disj*, int depth, bool sameline);
void ast_log_conj_print(const variant_ast_log_conj*, int depth, bool sameline);
void ast_rel_print(const variant_ast_rel*, int depth, bool sameline);
void ast_math_expr_print(const variant_ast_math_expr*, int depth, bool sameline);
void ast_term_print(const variant_ast_term*, int depth, bool sameline);
void ast_factor_print(const variant_ast_factor*, int depth, bool sameline);
#endif
