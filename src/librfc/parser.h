#ifndef PARSER_H
#define PARSER_H
#include "tokenizer.h"
#include "result.h"
#include "ast.h"
#include "stack_ast_node.h"
#include "arena.h"

typedef struct {
  variant_ast_node root;
  stack_ast_node ast_stack;
  int token_index;
  Arena ast_arena;
} parser_ctx;

typedef struct {
  variant_ast_node nodes[10];
  int count;
} check_result;
typedef struct {
  int checktype; // 1 is token, 2 is ast
  union {
    token_type tokentype;
    variant_ast_node_type astnodetype;
  };
} check;

define_result(stack_check, check_result, const char*);
define_result(parser_run, parser_ctx, const char*);

#define token(type)\
  (check){.checktype = 1, .tokentype = type}

#define ast(type)\
  (check){.checktype = 2, .astnodetype = type}

#define check_seq(...)\
  (check[])__VA_ARGS__, sizeof((check[])__VA_ARGS__)/sizeof(check)


result_stack_check stack_check(stack_ast_node* stack, check c[], int n);
result_parser_run parser_run(tokenizer* t);

#endif
