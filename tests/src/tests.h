#ifndef TESTS_H
#define TESTS_H
#include "../lib/munit.h"

MunitResult tokenizer_create_invldpath_test(const MunitParameter*, void*);
MunitResult tokenizer_create_validpath_test(const MunitParameter*, void*);
MunitResult tokenizer_run_test(const MunitParameter*, void*);

MunitResult stack_test_type_create_test(const MunitParameter *, void *);
MunitResult stack_test_type_push_test(const MunitParameter *, void *);
MunitResult stack_test_type_push_pop_test(const MunitParameter *, void *);
MunitResult stack_test_type_push_pop_n_test(const MunitParameter *, void *);
MunitResult stack_test_type_top_offset_test(const MunitParameter *, void *);
MunitResult stack_test_type_top_test(const MunitParameter *, void *);
MunitResult stack_test_type_empty_test(const MunitParameter *, void *);

MunitResult dynarray_token_create_test(const MunitParameter *, void *);
MunitResult dynarray_token_pushback_test(const MunitParameter *, void *);
MunitResult dynarray_token_pushback_grow_test(const MunitParameter *, void *);
MunitResult dynarray_token_at_test(const MunitParameter *, void *);

MunitResult token_type_str_test(const MunitParameter *, void *);

MunitResult result_err_test(const MunitParameter *, void *);
MunitResult result_ok_test(const MunitParameter *, void *);
MunitResult result_match_err_test(const MunitParameter *, void *);
MunitResult result_match_ok_test(const MunitParameter *, void *);

MunitResult parser_stack_check_test_1(const MunitParameter *, void *);
MunitResult parser_stack_check_test_2(const MunitParameter *, void *);
MunitResult parser_stack_check_passed_limit_test(const MunitParameter *, void *);
MunitResult parser_stack_check_stack_larger_than_check(const MunitParameter *, void *);

#endif
