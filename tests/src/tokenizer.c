#include "tests.h"
#include "../../src/librfc/tokenizer.h"

MunitResult tokenizer_create_invldpath_test(const MunitParameter *param, void *context){
  result_tokenizer_create t = tokenizer_create("");
  munit_assert_true(t.isfail);
  munit_assert_false(t.isok);
  tokenizer_free(&t.ok);
  return MUNIT_OK;
}

MunitResult tokenizer_create_validpath_test(const MunitParameter *param, void *context){
  result_tokenizer_create t = tokenizer_create("samples/main.rf");
  munit_assert_true(t.isok);
  munit_assert_false(t.isfail);
  return MUNIT_OK;
}

MunitResult tokenizer_run_test(const MunitParameter *param, void *context){
  result_tokenizer_create t = tokenizer_create("samples/main.rf");
  munit_assert_true(t.isok);
  munit_assert_false(t.isfail);

  result_tokenizer_run r = tokenizer_run(&t.ok);
  munit_assert_true(t.isok);

  tokenizer_free(&t.ok);
  return MUNIT_OK;
}
