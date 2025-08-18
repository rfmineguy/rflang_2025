#include "munit.h"

#include "tests.h"


void* setup(const MunitParameter params[], void* userdata) {
  return NULL;
}

void teardown(void* fixture) {}

MunitTest stack_test_type_tests[] = {
  { "/create",     stack_test_type_create_test, setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { "/push",       stack_test_type_push_test, setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { "/push_pop",   stack_test_type_push_pop_test, setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { "/push_popn",  stack_test_type_push_pop_n_test, setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { "/top",        stack_test_type_top_test, setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { "/top_offset", stack_test_type_top_offset_test, setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { "/empty",      stack_test_type_empty_test, setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { NULL, NULL,     NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};

MunitTest dynarray_token_tests[] = {
  { "/create",        dynarray_token_create_test, setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { "/push_back",     dynarray_token_pushback_test, setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { "/push_back_grow",dynarray_token_pushback_grow_test, setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { "/at",            dynarray_token_at_test, setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { NULL, NULL,     NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};

MunitTest tokenizer_tests[] = {
  { "/create_invld_path", tokenizer_create_invldpath_test, setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { "/create_valid_path", tokenizer_create_validpath_test, setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { "/run",               tokenizer_run_test, setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { NULL, NULL,     NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};

MunitTest token_tests[] = {
  { "/type_str",               token_type_str_test, setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { NULL, NULL,     NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};

MunitTest variant_tests[] = {
  { "/datatype",          variant_datatype_test, setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { "/datatype_Int",      variant_datatype_Int_match_test, setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { "/datatype_Float",    variant_datatype_Float_match_test, setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { "/datatype_String",   variant_datatype_String_match_test, setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { NULL, NULL,     NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};

MunitTest result_tests[] = {
  { "/err",               result_err_test, setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { "/ok",                result_ok_test, setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { "/match_err",         result_match_err_test, setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { "/match_ok",          result_match_ok_test, setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { NULL, NULL,     NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};

MunitTest parser_stack_tests[] = {
  { "/stack_check_1",                        parser_stack_check_test_1, setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { "/stack_check_2",                        parser_stack_check_test_2, setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { "/stack_check_pass_limit",               parser_stack_check_passed_limit_test, setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { "/stack_check_stack_larger_than_check",  parser_stack_check_stack_larger_than_check, setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { "/parser_stack_check_incorrect_ast_node",parser_stack_check_incorrect_ast_node, setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { "/reduce_stack_var_decl",                parser_reduce_stack_vardecl, setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { NULL, NULL,     NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};

MunitTest parser_run_tests[] = {
  { "/expr_single_number", parser_run_expr_single_number, setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { "/expr_simple_addition", parser_run_expr_simple_addition, setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { "/expr_add_and_mul", parser_run_expr_add_and_mul, setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { "/expr_with_parens", parser_run_expr_with_parens, setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { "/vardecl",          parser_run_vardecl, setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { NULL, NULL,     NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};

MunitSuite parser_suite[] = {
  { "/parse_stack",    parser_stack_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE },
  { "/run",            parser_run_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE },
  {NULL, NULL, NULL, 0, MUNIT_SUITE_OPTION_NONE},
};

MunitSuite all_tests[] = {
  { "/stack_test_type",stack_test_type_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE },
  { "/dynarray_token", dynarray_token_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE },
  { "/tokenizer",      tokenizer_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE },
  { "/token",          token_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE },
  { "/variant",        variant_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE },
  { "/result",         result_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE },
  { "/parser",         NULL, parser_suite, 1, MUNIT_SUITE_OPTION_NONE },
  {NULL, NULL, NULL, 0, MUNIT_SUITE_OPTION_NONE},
};

static const MunitSuite main_suite = {
  "rflang",
  NULL,
  all_tests,
  1,
  MUNIT_SUITE_OPTION_NONE
};

int main(int argc, const char** argv) {
  return munit_suite_main(&main_suite, NULL, argc, (char *const *)argv);
}
