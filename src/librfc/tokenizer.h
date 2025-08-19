#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP
#include "result.h"
#include "dynarray_token.h"

typedef struct {
  const char* filepath;
  const char* fileContents;
  bool isFileContentsMalloced;
  dynarray_token tokens;
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
