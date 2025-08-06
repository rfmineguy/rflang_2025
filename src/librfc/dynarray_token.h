#ifndef DYNARRAY_token_H
#define DYNARRAY_token_H
#include <stddef.h>
#include "result.h"
#include "token.h"

define_result(dynarray_token_at, token, const char*);

typedef struct {
  token *buffer;
  size_t size, capacity;
} dynarray_token;

void                  dynarray_token_resize(dynarray_token*, size_t);

dynarray_token           dynarray_token_create();
void                  dynarray_token_pushback(dynarray_token*, token);
result_dynarray_token_at dynarray_token_at(dynarray_token*, int);

#endif
