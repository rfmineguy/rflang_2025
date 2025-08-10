#ifndef AST_H
#define AST_H
#include "token.h"
#include "variant.h"

typedef struct { token t; } ast_token;
typedef struct { token id; token type; } ast_vardecl;
typedef struct { const char* placeholder; } ast_expr;

#define ast_node_variant(X)\
  X(Token, ast_node, ast_token)\
  X(Var,   ast_node, ast_vardecl)\
  X(Expr,  ast_node, ast_expr)

define_variant(ast_node, ast_node_variant);

#endif
