#include "tests.h"
#include "../lib/test_util.h"
#define TEMP_FILE "file.tmp"

MunitResult redirect_empty(const MunitParameter *param, void *context) {
  redirect_to_filename_begin(NULL, TEMP_FILE, NULL);
  redirect_to_filename_end();

  munit_assert_file_contents_equal(TEMP_FILE, "");
  return MUNIT_OK;
}

MunitResult redirect_oneline_newline(const MunitParameter *param, void *context) {
  redirect_to_filename_begin(NULL, TEMP_FILE, NULL);
    printf("Hello world\n");
  redirect_to_filename_end()

  munit_assert_file_contents_equal(TEMP_FILE, "Hello world\n");
  return MUNIT_OK;
}

MunitResult redirect_oneline_no_newline(const MunitParameter *param, void *context) {
  FILE* outfile = fopen(TEMP_FILE, "w");
  redirect_to_filename_begin(NULL, TEMP_FILE, NULL)
    printf("Hello world");
  redirect_to_filename_end()

  munit_assert_file_contents_equal(TEMP_FILE, "Hello world");
  return MUNIT_OK;
}

MunitResult redirect_twice(const MunitParameter *param, void *context) {
  redirect_to_filename_begin(NULL, TEMP_FILE, NULL)
  printf("Hello world");
  redirect_to_filename_end()

  redirect_to_filename_begin(NULL, TEMP_FILE, NULL)
    printf("Hi");
  redirect_to_filename_end()

  redirect_to_filename_begin(NULL, TEMP_FILE, NULL)
  printf("Here is another redirect");
  redirect_to_filename_end()

  munit_assert_file_contents_equal(TEMP_FILE, "Here is another redirect");
  return MUNIT_OK;
}
