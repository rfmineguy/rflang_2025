#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP
#include <regex.h>
#include "result.h"
#include "dynarray_token.h"

typedef struct {
  const char* filepath;
  const char* fileContents;
  dynarray_token tokens;

  struct {
    regex_t id, intlit, dbllit;
    regex_t arrow;
  } regex_store;
} tokenizer;

define_result(tokenizer_create, tokenizer, const char*)
define_result(tokenizer_run, void*, const char*)
define_result(read_file, char*, const char*)
define_result(compile_reg, void*, const char*)
define_result(test_reg, token, const char*)

result_tokenizer_create tokenizer_create_file(const char*);
result_tokenizer_create tokenizer_create_cstr(const char*);
void      tokenizer_free(tokenizer*);


result_tokenizer_run     tokenizer_run(tokenizer*);

#endif
