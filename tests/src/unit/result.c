#include "tests.h"
#include "../../src/librfc/result.h"
#include <stdio.h>

define_result(test, int, const char*)

MunitResult result_err_test(const MunitParameter *param, void *context) {
  result_test res = result_err(test, "This is a test error");
  munit_assert_true(res.isfail);
  munit_assert_false(res.isok);
  munit_assert_string_equal(res.err, "This is a test error");
  return MUNIT_OK;
}

MunitResult result_ok_test(const MunitParameter *param, void *context) {
  result_test res = result_ok(test, 4);
  munit_assert_true(res.isok);
  munit_assert_false(res.isfail);
  munit_assert_int(res.ok, ==, 4);
  return MUNIT_OK;
}

MunitResult result_match_err_test(const MunitParameter *param, void *context) {
  char format_err[100][10] = {0};
  for (int i = 0; i < 100; i++) {
    snprintf(format_err[i], 10, "err: %d", i);
  }
  for (int i = 0; i < 100; i++) {
    match(result_err(test, format_err[i]), test, munit_assert(false);, munit_assert_string_equal(result_.err, format_err[i]););
  }
  return MUNIT_OK;
}

MunitResult result_match_ok_test(const MunitParameter *param, void *context) {
  for (int i = 0; i < 100; i++) {
    match(result_ok(test, i), test, munit_assert_int(result_.ok, ==, i);, munit_assert(false););
  }
  return MUNIT_OK;
}
