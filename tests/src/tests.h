#ifndef TESTS_H
#define TESTS_H
#include "../lib/munit.h"

MunitResult tokenizer_create_test(const MunitParameter*, void*);
MunitResult tokenizer_free_test(const MunitParameter*, void*);
MunitResult tokenizer_run_test(const MunitParameter*, void*);

#endif
