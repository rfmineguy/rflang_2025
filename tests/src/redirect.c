#include "tests.h"
#include "../lib/test_util.h"
#define TEMP_FILE "file.tmp"

MunitResult redirect_empty(const MunitParameter *param, void *context) {
  redirect_begin(STDOUT_FILENO, TEMP_FILE, handle);
  redirect_end(STDOUT_FILENO, handle);

  munit_assert_file_contents_equal(TEMP_FILE, "");
  return MUNIT_OK;
}

MunitResult redirect_oneline_newline(const MunitParameter *param, void *context) {
  redirect_begin(STDOUT_FILENO, TEMP_FILE, handle);
  printf("Hello world\n");
  redirect_end(STDOUT_FILENO, handle);

  munit_assert_file_contents_equal(TEMP_FILE, "Hello world\n");
  return MUNIT_OK;
}
