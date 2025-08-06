#include "tokenizer.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
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

static result_test_reg test_seq(const char* needle, const char* str) {
  size_t needle_len = strlen(needle);
  if (strncmp(needle, str, needle_len) == 0) return result_ok(test_reg, ((token) {.start = str, .len = needle_len}));
  return result_err(test_reg, "String test failed");
}

static result_test_reg test_ceq(char c, const char* str) {
  if (*str == c) return result_ok(test_reg, ((token) {.start = str, .len = 1}));
  return result_err(test_reg, "Char test failed");
}

static result_test_reg test_strlit(const char* str) {
  if (!str) return result_err(test_reg, "str is NULL");
  if (*str != '"') return result_err(test_reg, "strlit does not begin with quote");
  const char* begin = str;
  str++;
  while (*str != '"') str++;
  str++;

  return result_ok(test_reg, ((token) {.start = begin, .len = str - begin}));
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

  return result_ok(tokenizer_create, t);
}

void tokenizer_free(tokenizer* tok) {
  free(tok->fileContents);
  regfree(&tok->regex_store.id);
  regfree(&tok->regex_store.intlit);
  regfree(&tok->regex_store.dbllit);
}

static int isnewline(const char* cursor) {
  return *cursor == '\n' ||
         *cursor == '\r' ||
         (*cursor == '\r' && *(cursor + 1) == '\n');
}

result_tokenizer_run tokenizer_run(tokenizer* tok) {
  const char* cursor = tok->fileContents;
  while (*cursor) {
    if (*cursor == ' ') { cursor++; continue; }
    if (isnewline(cursor)) { cursor ++; continue; }
    match(test_seq("if",     cursor), test_reg,                { result_.ok.type = KEYWORD; print_token(result_.ok); cursor += result_.ok.len; }, {});
    match(test_seq("else",   cursor), test_reg,                { result_.ok.type = KEYWORD; print_token(result_.ok); cursor += result_.ok.len; }, {});
    match(test_seq("return", cursor), test_reg,                { result_.ok.type = KEYWORD; print_token(result_.ok); cursor += result_.ok.len; }, {});
    match(test_seq("->",     cursor), test_reg,                { result_.ok.type = ARROW; print_token(result_.ok); cursor += result_.ok.len; }, {});
    match(test_strlit(cursor),    test_reg,                    { result_.ok.type = STRLIT;  print_token(result_.ok); cursor += result_.ok.len; }, {});
    match(test_reg(&tok->regex_store.id,    cursor), test_reg, { result_.ok.type = ID;      print_token(result_.ok); cursor += result_.ok.len; }, {});
    match(test_reg(&tok->regex_store.intlit,cursor), test_reg, { result_.ok.type = INTLIT;  print_token(result_.ok); cursor += result_.ok.len; }, {});
    match(test_ceq('=',  cursor), test_reg,                     { result_.ok.type = EQ;     print_token(result_.ok); cursor += result_.ok.len; }, {});
    match(test_ceq('+',  cursor), test_reg,                     { result_.ok.type = PLUS;   print_token(result_.ok); cursor += result_.ok.len; }, {});
    match(test_ceq('-',  cursor), test_reg,                     { result_.ok.type = MINUS;  print_token(result_.ok); cursor += result_.ok.len; }, {});
    match(test_ceq('%',  cursor), test_reg,                     { result_.ok.type = MOD;    print_token(result_.ok); cursor += result_.ok.len; }, {});
    match(test_ceq('*',  cursor), test_reg,                     { result_.ok.type = MUL;    print_token(result_.ok); cursor += result_.ok.len; }, {});
    match(test_ceq('/',  cursor), test_reg,                     { result_.ok.type = DIV;    print_token(result_.ok); cursor += result_.ok.len; }, {});
    match(test_ceq(':',  cursor), test_reg,                     { result_.ok.type = COLON;  print_token(result_.ok); cursor += result_.ok.len; }, {});
    match(test_ceq(',',  cursor), test_reg,                     { result_.ok.type = COMMA;  print_token(result_.ok); cursor += result_.ok.len; }, {});
    match(test_ceq('{',  cursor), test_reg,                     { result_.ok.type = LCBRK;  print_token(result_.ok); cursor += result_.ok.len; }, {});
    match(test_ceq('}',  cursor), test_reg,                     { result_.ok.type = RCBRK;  print_token(result_.ok); cursor += result_.ok.len; }, {});
    match(test_ceq('[',  cursor), test_reg,                     { result_.ok.type = LSBRK;  print_token(result_.ok); cursor += result_.ok.len; }, {});
    match(test_ceq(']',  cursor), test_reg,                     { result_.ok.type = RSBRK;  print_token(result_.ok); cursor += result_.ok.len; }, {});
    match(test_ceq('(',  cursor), test_reg,                     { result_.ok.type = LPAR;   print_token(result_.ok); cursor += result_.ok.len; }, {});
    match(test_ceq(')',  cursor), test_reg,                     { result_.ok.type = RPAR;   print_token(result_.ok); cursor += result_.ok.len; }, {});
    match(test_seq("<=", cursor), test_reg,                     { result_.ok.type = LTEQ;   print_token(result_.ok); cursor += result_.ok.len; }, {});
    match(test_ceq('<',  cursor), test_reg,                     { result_.ok.type = LT;     print_token(result_.ok); cursor += result_.ok.len; }, {});
    match(test_seq(">=", cursor), test_reg,                     { result_.ok.type = GTEQ;   print_token(result_.ok); cursor += result_.ok.len; }, {});
    match(test_ceq('>',  cursor), test_reg,                     { result_.ok.type = GT;     print_token(result_.ok); cursor += result_.ok.len; }, {});
  }
  return result_ok(tokenizer_run, "");
}
