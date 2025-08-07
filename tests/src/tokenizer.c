#include "tests.h"
#include "../../src/librfc/tokenizer.h"

MunitResult tokenizer_create_invldpath_test(const MunitParameter *param, void *context){
  result_tokenizer_create t = tokenizer_create_file("");
  munit_assert_true(t.isfail);
  munit_assert_false(t.isok);
  tokenizer_free(&t.ok);
  return MUNIT_OK;
}

MunitResult tokenizer_create_validpath_test(const MunitParameter *param, void *context){
  result_tokenizer_create t = tokenizer_create_file("samples/main.rf");
  munit_assert_true(t.isok);
  munit_assert_false(t.isfail);
  return MUNIT_OK;
}

MunitResult tokenizer_run_test(const MunitParameter *param, void *context){
  result_tokenizer_create t = tokenizer_create_file("samples/tokenizer_test.rf");
  munit_assert_true(t.isok);
  munit_assert_false(t.isfail);

  result_tokenizer_run r = tokenizer_run(&t.ok);
  munit_assert_true(t.isok);

  munit_assert_int(t.ok.tokens.size, ==, 13);
  munit_assert_int(t.ok.tokens.buffer[0].type , ==, ID);
  munit_assert_int(t.ok.tokens.buffer[1].type , ==, EQ);
  munit_assert_int(t.ok.tokens.buffer[2].type , ==, COLON);
  munit_assert_int(t.ok.tokens.buffer[3].type , ==, COMMA);
  munit_assert_int(t.ok.tokens.buffer[4].type , ==, MUL);
  munit_assert_int(t.ok.tokens.buffer[5].type , ==, ARROW);
  munit_assert_int(t.ok.tokens.buffer[6].type , ==, ID);
  munit_assert_int(t.ok.tokens.buffer[7].type , ==, LCBRK);
  munit_assert_int(t.ok.tokens.buffer[8].type , ==, KEYWORD);
  munit_assert_int(t.ok.tokens.buffer[9].type , ==, LT);
  munit_assert_int(t.ok.tokens.buffer[10].type, ==, GT);
  munit_assert_int(t.ok.tokens.buffer[11].type, ==, INTLIT);
  munit_assert_int(t.ok.tokens.buffer[12].type, ==, KEYWORD);

  tokenizer_free(&t.ok);
  return MUNIT_OK;
}
