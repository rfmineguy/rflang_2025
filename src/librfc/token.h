#ifndef TOKEN_H
#define TOKEN_H
#include <stddef.h>

typedef enum {
  KEYWORD, EOF_,
  ID, EQ, COLON,
  COMMA, SQUOT, DQUOT,
  PLUS, MINUS, MUL, DIV, MOD,
  ARROW,
  LCBRK, RCBRK,
  LSBRK, RSBRK,
  LPAR, RPAR,
  LT, LTEQ,
  GT, GTEQ,
  DEQ,
  DAND, DOR,
  INTLIT, STRLIT,
  TOKEN_TYPE_LAST,
} token_type;

typedef struct {
  token_type type;
  const char* start;
  size_t len;
} token;

void        token_print(token t); 
const char* token_type_str(token_type tt);


#endif
