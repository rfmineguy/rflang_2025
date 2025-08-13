#include "ast.h"
#include <stdio.h>
#define INDENT_FMT "%*.s"
#define INDENT_ARGS depth * 2, " "
#define TOKEN_ARGS(sv) (int) sv.len, sv.start

void ast_token_print(ast_token t, int depth) {
  printf(INDENT_FMT "Token(%s), '%.*s'\n", INDENT_ARGS, token_type_str(t.t.type), TOKEN_ARGS(t.t));
}
