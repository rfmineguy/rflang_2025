#ifndef TESTS_H
#define TESTS_H
#include "../lib/munit.h"

MunitResult tokenizer_create_invldpath_test(const MunitParameter*, void*);
MunitResult tokenizer_create_validpath_test(const MunitParameter*, void*);
MunitResult tokenizer_run_test(const MunitParameter*, void*);

MunitResult stack_ast_node_create_test(const MunitParameter *, void *);
MunitResult stack_ast_node_push_test1(const MunitParameter *, void *);
MunitResult stack_ast_node_push_pop_test(const MunitParameter *, void *);

MunitResult dynarray_token_create_test(const MunitParameter *, void *);
MunitResult dynarray_token_pushback_test(const MunitParameter *, void *);
MunitResult dynarray_token_pushback_grow_test(const MunitParameter *, void *);
MunitResult dynarray_token_at_test(const MunitParameter *, void *);

MunitResult token_type_str_test(const MunitParameter *, void *);

MunitResult result_err_test(const MunitParameter *, void *);
MunitResult result_ok_test(const MunitParameter *, void *);
MunitResult result_match_err_test(const MunitParameter *, void *);
MunitResult result_match_ok_test(const MunitParameter *, void *);

#endif
