#include "dynarray_token.h"
#include <stdlib.h>

void dynarray_token_resize(dynarray_token* da, size_t new) {
  da->capacity = new;
  da->buffer = realloc(da->buffer, new * sizeof(token));
}

dynarray_token dynarray_token_create() {
  dynarray_token a = (dynarray_token) {
    .buffer = 0,
    .size = 0,
    .capacity = 10,
  };
  a.buffer = calloc(a.capacity, sizeof(token));
  return a;
}
void dynarray_token_pushback(dynarray_token* da, token val) {
  if (da->size + 1 >= da->capacity) dynarray_token_resize(da, da->capacity * 2);
  da->buffer[da->size++] = val;
}
result_dynarray_token_at dynarray_token_at(dynarray_token* da, int index) {
  if (index > da->size) return result_err(dynarray_token_at, "index out of range");
  return result_ok(dynarray_token_at, da->buffer[index]);
}
