#ifndef AST_H
#define AST_H

typedef enum {
  AST_IDENT, AST_INTLIT,
} ast_node_type;

typedef struct {
  ast_node_type type;

  union {
    struct { const char* s; int len; } ident;
    struct { int val; } intlit;
  };
} ast_node;

#endif
