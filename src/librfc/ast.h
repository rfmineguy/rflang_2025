#ifndef AST_H
#define AST_H

#include "token.h"
#include "variant.h"
forward_dec_variant(ast_expr);
forward_dec_variant(ast_node);
forward_dec_variant(ast_lit);

typedef struct { token t; } ast_token;
typedef struct { token id; token type; } ast_vardecl;
// typedef struct { struct variant_ast_expr* expr; } ast_expr;

/*
 * ==============
 * Variant setup
 * ==============
 */
#define ast_expr_variant(X)\
  X(ParenExpr, ast_expr, expr_paren)\
  X(LitExpr, ast_expr, expr_lit)\
  X(LitPlusExpr, ast_expr, expr_lit_plus_expr)\
  X(LitMinusExpr, ast_expr, expr_lit_minus_expr)\

#define ast_node_variant(X)\
  X(Token, ast_node, ast_token)\
  X(Var,   ast_node, ast_vardecl)\
  X(VariantExpr,  ast_node, variant_ast_expr*)\
  X(VariantLit,  ast_node, variant_ast_lit*)\

#define ast_lit_variant(X)\
  X(Int, ast_lit, token)\
  X(Double, ast_lit, token)\

typedef struct { variant_ast_expr* expr; } expr_paren;
typedef struct { variant_ast_expr* expr; variant_ast_lit* lit; } expr_lit_plus_expr;
typedef struct { variant_ast_expr* expr; variant_ast_lit* lit; } expr_lit_minus_expr;
typedef struct { variant_ast_lit* lit; } expr_lit;

define_variant(ast_expr, ast_expr_variant)
define_variant(ast_node, ast_node_variant)
define_variant(ast_lit,  ast_lit_variant)

void ast_lit_print(const variant_ast_lit* n, int depth);
void ast_expr_print(const variant_ast_expr* n, int depth);
void ast_node_print(variant_ast_node n, int depth);

#endif
