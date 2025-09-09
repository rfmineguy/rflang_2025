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

MunitResult redirect_oneline_no_newline(const MunitParameter *param, void *context) {
  redirect_begin(STDOUT_FILENO, TEMP_FILE, handle);
  printf("Hello world");
  redirect_end(STDOUT_FILENO, handle);

  munit_assert_file_contents_equal(TEMP_FILE, "Hello world");
  return MUNIT_OK;
}

MunitResult redirect_twice(const MunitParameter *param, void *context) {
  redirect_begin(STDOUT_FILENO, TEMP_FILE, handle);
  printf("Hello world");
  redirect_end(STDOUT_FILENO, handle);
  munit_assert_file_contents_equal(TEMP_FILE, "Hello world");

  redirect_begin(STDOUT_FILENO, TEMP_FILE, handle2);
  printf("Here is another redirect");
  redirect_end(STDOUT_FILENO, handle2);

  munit_assert_file_contents_equal(TEMP_FILE, "Here is another redirect");
  return MUNIT_OK;
}
