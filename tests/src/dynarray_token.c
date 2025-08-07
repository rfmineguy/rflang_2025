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
