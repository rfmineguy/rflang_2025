#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP
#include <regex.h>
#include "result.h"

typedef struct {
  const char* filepath;
  char* fileContents;

  struct {
    regex_t id, intlit, dbllit;
  } regex_store;
} tokenizer;

define_result(tokenizer_create, tokenizer, const char*)
define_result(tokenizer_run, void*, const char*)
define_result(read_file, char*, const char*)

result_tokenizer_create tokenizer_create(const char*);
void      tokenizer_free(tokenizer*);


result_tokenizer_run     tokenizer_run(tokenizer*);

#endif
