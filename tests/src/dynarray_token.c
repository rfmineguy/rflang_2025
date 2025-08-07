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

MunitResult dynarray_token_pushback_grow_test(const MunitParameter * params, void *context) {
  const char* ids[20] = {
    "ear",
    "volcano",
    "witch",
    "clique",
    "contract",
    "student",
    "decline",
    "concentration",
    "wriggle",
    "apathy",
    "transaction",
    "is",
    "multimedia",
    "shrink",
    "shed",
    "observer",
    "contempt",
    "produce",
    "ritual",
    "leash",
  };

  dynarray_token da = dynarray_token_create();
  munit_assert_int(da.size, ==, 0);
  munit_assert_int(da.capacity, ==, 10);

  // test if growing works properly
  for (int i = 0; i < 20; i++) dynarray_token_pushback(&da, (token){.type = ID, .start = ids[i], .len = strlen(ids[i])});
  munit_assert_int(da.size, ==, 20);
  munit_assert_int(da.capacity, ==, 40);

  // test if the elements are correct
  for (int i = 0; i < 20; i++) {
    result_dynarray_token_at r = dynarray_token_at(&da, i);
    munit_assert_true(r.isok);

    munit_assert_string_equal(ids[i], r.ok.start);
  }

  dynarray_token_free(&da);
  return MUNIT_OK;
}
