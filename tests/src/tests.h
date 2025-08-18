#ifndef TESTS_H
#define TESTS_H
#include "munit.h"
#include "munit_ext.h"

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

MunitResult variant_datatype_test(const MunitParameter *, void *);
MunitResult variant_datatype_Int_match_test(const MunitParameter *, void *);
MunitResult variant_datatype_Float_match_test(const MunitParameter *, void *);
MunitResult variant_datatype_String_match_test(const MunitParameter *, void *);

MunitResult parser_stack_check_test_1(const MunitParameter *, void *);
MunitResult parser_stack_check_test_2(const MunitParameter *, void *);
MunitResult parser_stack_check_passed_limit_test(const MunitParameter *, void *);
MunitResult parser_stack_check_stack_larger_than_check(const MunitParameter *, void *);
MunitResult parser_stack_check_incorrect_ast_node(const MunitParameter *, void *);
MunitResult parser_reduce_stack_vardecl(const MunitParameter *, void *);

MunitResult parser_run_expr_single_number(const MunitParameter *, void *);
MunitResult parser_run_expr_simple_addition(const MunitParameter *, void *);
MunitResult parser_run_expr_add_and_mul(const MunitParameter *, void *);
MunitResult parser_run_expr_with_parens(const MunitParameter *, void *);
MunitResult parser_run_vardecl(const MunitParameter *, void *);

#endif
