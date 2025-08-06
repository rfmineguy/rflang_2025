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

static result_read_file ReadFile(const char* filepath) {
  FILE* f = fopen(filepath, "r");
  if (!f) {
    fprintf(stderr, "Failed to open '%s'\n", filepath);
    return result_err(read_file, "Failed to open file");
  }

  fseek(f, 0, SEEK_END);
  size_t len = ftell(f);
  fseek(f, 0, SEEK_SET);

  char* buf = calloc(len, sizeof(char));
  size_t read;
  if ((read = fread(buf, sizeof(char), len, f)) != len) {
    fprintf(stderr, "Failed to read entire file '%s'. Read %zu/%zu bytes\n", filepath, read, len);
    fclose(f);
    free(buf);
    return result_err(read_file, "Failed to read entire file");
  }

  return result_ok(read_file, buf);
}
