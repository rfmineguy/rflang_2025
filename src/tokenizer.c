#include "tokenizer.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
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

static result_test_reg test_reg(regex_t* regex, const char* str) {
  regmatch_t match[1];
  if (regexec(regex, str, 1, match, 0) == 0) {
    if (match[0].rm_so != 0) {
      return result_err(test_reg, "No matches");
    }
    const char* start = str + match[0].rm_so;
		const char* end = str + match[0].rm_eo;
		int len = end - start;
		return result_ok(test_reg, ((token) {.start = start, .len = len}));
  }
  return result_err(test_reg, "Failed to exec");
}

static result_test_reg test_eq(char c, const char* str) {
  if (*str == c) return result_ok(test_reg, ((token) {.start = str, .len = 1}));
  return result_err(test_reg, "Char test failed");
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
    case COMMA:   return "COMMA";
    case ARROW:   return "ARROW";
    default:      assert(0 && "This type doesn't have a string yet...");
  }
  assert(0 && "Unreachable");
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

void tokenizer_free(tokenizer* tok) {
  free(tok->fileContents);
  regfree(&tok->regex_store.id);
  regfree(&tok->regex_store.intlit);
  regfree(&tok->regex_store.dbllit);
  regfree(&tok->regex_store.arrow);
}

static void print_token(token t) {
  printf("Token { %s, %.*s }\n", token_type_str(t.type), (int)t.len, t.start);
}
