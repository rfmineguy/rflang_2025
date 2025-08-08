#ifndef AST_H
#define AST_H
#include "token.h"

typedef enum {
  AST_TOKEN, AST_VAR
} ast_node_type;

typedef struct {
  ast_node_type type;

  union {
    struct { token t; } token;
    struct { int placeholder; } var;
  };
} ast_node;

#endif
