#include "tests.h"
#include "../../src/librfc/tokenizer.h"

MunitResult tokenizer_create_invldpath_test(const MunitParameter *param, void *context){
  result_tokenizer_create t = tokenizer_create("");
  munit_assert_true(t.isfail);
  munit_assert_false(t.isok);
  return MUNIT_OK;
}

MunitResult tokenizer_create_validpath_test(const MunitParameter *param, void *context){
  result_tokenizer_create t = tokenizer_create("samples/main.rf");
  munit_assert_true(t.isok);
  munit_assert_false(t.isfail);
  return MUNIT_OK;
}

MunitResult tokenizer_free_test(const MunitParameter *param, void *context){
  return MUNIT_ERROR;
}

MunitResult tokenizer_run_test(const MunitParameter *param, void *context){
  return MUNIT_ERROR;
}
