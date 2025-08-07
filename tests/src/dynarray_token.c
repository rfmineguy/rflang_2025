#include "tests.h"
#include "../../src/librfc/dynarray_token.h"

MunitResult dynarray_token_create_test(const MunitParameter * params, void *context) {
  dynarray_token da = dynarray_token_create();
  munit_assert_not_null(da.buffer);
  munit_assert_int(da.size, ==, 0);
  munit_assert_int(da.capacity, ==, 10);

  dynarray_token_free(&da);
  return MUNIT_OK;
}

MunitResult dynarray_token_pushback_test(const MunitParameter * params, void *context) {
  const char* id_content = "hello";
  const char* strlit_content = "\"this is a string literal\"";
  dynarray_token da = dynarray_token_create();

  dynarray_token_pushback(&da, (token){.type = ID, .start = id_content, .len = strlen(id_content) });
  munit_assert_int(da.size, ==, 1);
  munit_assert_int(da.buffer[0].type, ==, ID);
  munit_assert_int(da.buffer[0].len, ==, strlen(id_content));
  munit_assert_ptr_equal(da.buffer[0].start, id_content);

  dynarray_token_pushback(&da, (token){.type = STRLIT, .start = strlit_content, .len = strlen(strlit_content) });
  munit_assert_int(da.size, ==, 2);
  munit_assert_int(da.buffer[1].type, ==, STRLIT);
  munit_assert_int(da.buffer[1].len, ==, strlen(strlit_content));
  munit_assert_ptr_equal(da.buffer[1].start, strlit_content);

  dynarray_token_free(&da);
  return MUNIT_OK;
}
