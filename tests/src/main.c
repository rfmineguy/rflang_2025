#include "../lib/munit.h"
#include "tests.h"

void* setup(const MunitParameter params[], void* userdata) {
  return NULL;
}

void teardown(void* fixture) {}

MunitTest stack_ast_node_tests[] = {
  { "/create",     stack_ast_node_create_test, setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { "/push",       stack_ast_node_push_test1, setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { "/push_pop",   stack_ast_node_push_pop_test, setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { "/top",        stack_ast_node_top_test, setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { "/top_offset", stack_ast_node_top_offset_test, setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
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
  { "/type_str",               tokenizer_run_test, setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { NULL, NULL,     NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};

MunitTest result_tests[] = {
  { "/err",               result_err_test, setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { "/ok",                result_ok_test, setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { "/match_err",         result_match_err_test, setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { "/match_ok",          result_match_ok_test, setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { NULL, NULL,     NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};

MunitSuite all_tests[] = {
  { "/stack_ast_node", stack_ast_node_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE },
  { "/dynarray_token", dynarray_token_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE },
  { "/tokenizer",      tokenizer_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE },
  { "/token",          token_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE },
  { "/result",         result_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE },
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
