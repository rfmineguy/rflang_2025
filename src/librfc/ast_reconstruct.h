#ifndef AST_RECONSTRUCT_H
#define AST_RECONSTRUCT_H
#include "ast.h"

void ast_var_reconstruct(const variant_ast_vardecl*);
void ast_type_reconstruct(const variant_ast_type*);
void ast_lit_reconstruct(const variant_ast_lit*);
void ast_expr_reconstruct(const variant_ast_expr*);
void ast_log_disj_reconstruct(const variant_ast_log_disj*);
void ast_log_conj_reconstruct(const variant_ast_log_conj*);
void ast_rel_reconstruct(const variant_ast_rel*);
void ast_math_expr_reconstruct(const variant_ast_math_expr*);
void ast_term_reconstruct(const variant_ast_term*);
void ast_factor_reconstruct(const variant_ast_factor*);
void ast_reconstruct(variant_ast_node);

#endif
