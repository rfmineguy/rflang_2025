#include "../lib/munit.h"
#include "tests.h"

void* setup(const MunitParameter params[], void* userdata) {
  return NULL;
}

void teardown(void* fixture) {}

MunitTest stack_ast_node_tests[] = {
  { "/create",   stack_ast_node_create_test, setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { "/push",     stack_ast_node_push_test1, setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { "/push_pop", stack_ast_node_push_pop_test, setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { NULL, NULL,     NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};

MunitTest tokenizer_tests[] = {
  { "/create_invld_path", tokenizer_create_invldpath_test, setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { "/create_valid_path", tokenizer_create_validpath_test, setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { "/run",               tokenizer_run_test, setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { NULL, NULL,     NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};

MunitSuite all_tests[] = {
  { "/stack_ast_node", stack_ast_node_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE },
  { "/tokenizer",      tokenizer_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE },
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
