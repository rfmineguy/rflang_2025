#include "tokenizer.h"
#include <stdlib.h>
#include <stdio.h>
static char msgbuf[500];

#define compile_reg(target, regstr)\
{\
  int r = regcomp(&target, regstr, REG_EXTENDED);\
  if (r) {\
    regerror(r, &target, msgbuf, sizeof(msgbuf));\
    fprintf(stderr, "compile_reg fail: %s\n", msgbuf);\
    fprintf(stderr, "  %s\n", regstr);\
    exit(1);\
  }\
  else {\
    printf("[Regex OK] %s\n", regstr);\
  }\
};
