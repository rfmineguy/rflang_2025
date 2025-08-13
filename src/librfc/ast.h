#ifndef AST_H
#define AST_H

#include "token.h"
#include "variant.h"
forward_dec_variant(ast_expr);
forward_dec_variant(ast_node);

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
  X(LitPlusExpr, ast_expr, expr_lit_plus_expr)\
  X(LitMinusExpr, ast_expr, expr_lit_minus_expr)\

#define ast_node_variant(X)\
  X(Token, ast_node, ast_token)\
  X(Var,   ast_node, ast_vardecl)\
  X(VariantExpr,  ast_node, variant_ast_expr*)

typedef struct { variant_ast_expr* expr; } expr_paren;
typedef struct { variant_ast_expr* expr; int lit; } expr_lit_plus_expr;
typedef struct { variant_ast_expr* expr; int lit; } expr_lit_minus_expr;

define_variant(ast_expr, ast_expr_variant)
define_variant(ast_node, ast_node_variant)

void ast_node_print(variant_ast_node n, int depth);

#endif
