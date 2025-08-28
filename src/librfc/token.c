#include "token.h"
#include <assert.h>
#include <stdio.h>

void token_print(token t) {
  printf("Token { '%s', '%.*s' }\n", token_type_str(t.type), (int)t.len, t.start);
}

const char* token_type_str(token_type tt) {
  switch (tt) {
    case ID:      return "ID";
    case EQ:      return "EQ";
    // Operators
    case PLUS:    return "PLUS";
    case MINUS:   return "MINUS";
    case MUL:     return "MUL";
    case DIV:     return "DIV";
    case MOD:     return "MOD";
    // Separators
    case COLON:   return "COLON";
    case SEMI:    return "SEMI";
    case COMMA:   return "COMMA";
    case SQUOT:   return "SQUOT";
    case DQUOT:   return "DQUOT";
    case ARROW:   return "ARROW";
    case KEYWORD: return "KEYWORD";

    case LCBRK:   return "LCBRK";
    case RCBRK:   return "RCBRK";
    case LSBRK:   return "LSBRK";
    case RSBRK:   return "RSBRK";
    case LPAR:    return "LPAR";
    case RPAR:    return "RPAR";

    case LT:      return "LT";
    case LTEQ:    return "LTEQ";
    case GT:      return "GT";
    case GTEQ:    return "GTEQ";

    case INTLIT:  return "INTLIT";
    case STRLIT:  return "STRLIT";
    case EOF_:    return "EOF_";

    case DEQ:     return "DEQ";
    case DAND:    return "DAND";
    case DOR:     return "DOR";
    default:      return "<UNDEFINED TOKEN TYPE>";
  }
  assert(0 && "Unreachable");
}

bool token_is_operator(token t) {
  return t.type == LT || t.type == LTEQ || t.type == GT ||
    t.type == GTEQ || t.type == DEQ || t.type == DAND ||
    t.type == DOR || t.type == PLUS || t.type == MINUS ||
    t.type == MUL || t.type == DIV || t.type == MOD;
}
