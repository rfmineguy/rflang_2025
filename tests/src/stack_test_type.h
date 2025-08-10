#ifndef STACK_test_type_H
#define STACK_test_type_H
#include <stddef.h>
#include "result.h"
#include "test_type.h"

define_result(stack_test_type_top, test_type, const char*);
define_result(stack_test_type_pop, void*, const char*);
define_result(stack_test_type_pop_n, void*, const char*);

typedef struct {
  test_type *buffer;
  int size, capacity;
} stack_test_type;

void                   stack_test_type_resize(stack_test_type*, size_t);

stack_test_type               stack_test_type_create();
void                   stack_test_type_free(stack_test_type*);
void                   stack_test_type_push(stack_test_type*, test_type);
result_stack_test_type_pop    stack_test_type_pop(stack_test_type*);
result_stack_test_type_pop_n  stack_test_type_pop_n(stack_test_type*, int);
result_stack_test_type_top    stack_test_type_top(stack_test_type*);
result_stack_test_type_top    stack_test_type_top_offset(stack_test_type*, int);
int                    stack_test_type_empty(stack_test_type*);

#endif
