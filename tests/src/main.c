#include "../lib/munit.h"
#include "tests.h"

void* setup(const MunitParameter params[], void* userdata) {
  return NULL;
}

void teardown(void* fixture) {}

MunitTest tokenizer_tests[] = {
  { "/create_invld_path", tokenizer_create_invldpath_test, setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { "/create_valid_path", tokenizer_create_validpath_test, setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { "/run", tokenizer_run_test, setup, teardown, MUNIT_TEST_OPTION_NONE, NULL},
  { NULL, NULL,     NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};

static const MunitSuite main_suite = {
  "rflang/tokenizer",
  tokenizer_tests,
  NULL,
  1,
  MUNIT_SUITE_OPTION_NONE
};

int main(int argc, const char** argv) {
  return munit_suite_main(&main_suite, NULL, argc, (char *const *)argv);
}
