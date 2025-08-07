#ifndef PARSER_H
#define PARSER_H
#include "tokenizer.h"
#include "result.h"
#include "ast.h"
#include "stack_ast_node.h"

typedef struct {
  ast_node root;
  stack_ast_node ast_stack;
} parser_ctx;

define_result(parser_run, parser_ctx, const char*);

result_parser_run parser_run(tokenizer* t);

#endif
