#include "tokenizer.h"
#include <stdlib.h>
#include <stdio.h>
static char msgbuf[500];

static result_compile_reg compile_reg(regex_t* target, const char* regstr) {
  int r = regcomp(target, regstr, REG_EXTENDED);
  if (r) {
    regerror(r, target, msgbuf, sizeof(msgbuf));
    fprintf(stderr, "compile_reg fail: %s\n", msgbuf);
    fprintf(stderr, "  %s\n", regstr);
    return result_err(compile_reg, "compile_reg fail");
  }
  else {
    printf("[Regex OK] %s\n", regstr);
    return result_ok(compile_reg, "");
  }
}


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

result_tokenizer_create tokenizer_create(const char *filepath) {
  tokenizer t = (tokenizer) {
    .filepath = filepath,
    .regex_store = {}
  };
  match(ReadFile(filepath), read_file, t.fileContents = result_.ok;, {
      return result_err(tokenizer_create, result_.err);
  });
  match(compile_reg(&t.regex_store.id,       "[_a-zA-Z][a-zA-Z0-9_]*"), compile_reg, {}, { return result_err(tokenizer_create, result_.err); });
  match(compile_reg(&t.regex_store.intlit,   "[0-9]+"),                 compile_reg, {}, { return result_err(tokenizer_create, result_.err); });
  match(compile_reg(&t.regex_store.dbllit,   "[1-9]+\\.[0-9]*"),        compile_reg, {}, { return result_err(tokenizer_create, result_.err); });
  match(compile_reg(&t.regex_store.arrow,    "->"),                     compile_reg, {}, { return result_err(tokenizer_create, result_.err); });

  return result_ok(tokenizer_create, t);
}
