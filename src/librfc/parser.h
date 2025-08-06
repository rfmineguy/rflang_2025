#ifndef PARSER_H
#define PARSER_H
#include "tokenizer.h"
#include "result.h"
#include "ast.h"

typedef struct {
  ast_node root;
} parser_ctx;

define_result(parser_run, parser_ctx, const char*);

result_parser_run parser_run(tokenizer* t);

#endif
