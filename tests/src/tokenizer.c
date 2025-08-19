#include "tests.h"
#include "../../src/librfc/tokenizer.h"

#define token_(type_, len_, content_)\
  (token) {.type = type_, .len = len_, .start = content_}

MunitResult tokenizer_create_invldpath_test(const MunitParameter *param, void *context){
#define INVALID_FILEPATHS_COUNT 3

  // Array of invalid filepaths to test
  const char* invalid_filepaths[INVALID_FILEPATHS_COUNT] = {
    "thisisafile.rf",
    "por/adsf.rf",
    "ads.rf",
  };

  // Make sure that the array length is correct
  munit_assert_int(sizeof(invalid_filepaths) / sizeof(const char*), ==, INVALID_FILEPATHS_COUNT);
  for (int i = 0; i < INVALID_FILEPATHS_COUNT; i++) {
    result_tokenizer_create t = tokenizer_create_file(invalid_filepaths[i]);
    munit_assert_true(t.isfail);
    munit_assert_false(t.isok);
    tokenizer_free(&t.ok);
  }

  return MUNIT_OK;
}

MunitResult tokenizer_create_validpath_test(const MunitParameter *param, void *context){
#define VALID_FILEPATHS_COUNT 2

  // Array of invalid filepaths to test
  const char* valid_filepaths[VALID_FILEPATHS_COUNT] = {
    "samples/main.rf",
    "samples/tokenizer_test.rf",
  };

  // Make sure that the array length is correct
  munit_assert_int(sizeof(valid_filepaths) / sizeof(const char*), ==, VALID_FILEPATHS_COUNT);
  for (int i = 0; i < VALID_FILEPATHS_COUNT; i++) {
    result_tokenizer_create t = tokenizer_create_file(valid_filepaths[i]);
    munit_assert_true(t.isok);
    munit_assert_false(t.isfail);
    tokenizer_free(&t.ok);
  }
  return MUNIT_OK;
}

MunitResult tokenizer_run_file_test(const MunitParameter *param, void *context){
#define EXPECTED_TOKENS_COUNT 14
  token expected_tokens[EXPECTED_TOKENS_COUNT] = {
    token_(ID     , 4, "main"),
    token_(EQ     , 1, "="),
    token_(COLON  , 1, ":"),
    token_(COMMA  , 1, ","),
    token_(MUL    , 1, "*"),
    token_(ARROW  , 2, "->"),
    token_(ID     , 3, "int"),
    token_(LCBRK  , 1, "{"),
    token_(KEYWORD, 2, "if"),
    token_(LT     , 1, "<"),
    token_(GT     , 1, ">"),
    token_(INTLIT , 1, "1"),
    token_(KEYWORD, 2, "if"),
    token_(EOF_   , 0, ""),
  };
  munit_assert_int(sizeof(expected_tokens) / sizeof(token), ==, EXPECTED_TOKENS_COUNT);

  result_tokenizer_create t = tokenizer_create_file("samples/tokenizer_test.rf");
  munit_assert_true(t.isok);
  munit_assert_false(t.isfail);

  result_tokenizer_run r = tokenizer_run(&t.ok);
  munit_assert_true(t.isok);

  munit_assert_int(EXPECTED_TOKENS_COUNT, ==, t.ok.tokens.size);

  for (int i = 0; i < t.ok.tokens.size; i++) {
    token actual = t.ok.tokens.buffer[i];
    token expected = expected_tokens[i];

    // assert each field of the token
    munit_assert_int(actual.type, ==, expected.type);
    munit_assert_int(actual.len, ==, expected.len);
    munit_assert_string_n_equal(actual.start, expected.start, actual.len);
  }

  tokenizer_free(&t.ok);
  return MUNIT_OK;
}

MunitResult tokenizer_run_cstr_test(const MunitParameter *param, void *context){
#define EXPECTED_TOKENS_COUNT 14
  token expected_tokens[EXPECTED_TOKENS_COUNT] = {
    token_(ID     , 4, "main"),
    token_(EQ     , 1, "="),
    token_(COLON  , 1, ":"),
    token_(COMMA  , 1, ","),
    token_(MUL    , 1, "*"),
    token_(ARROW  , 2, "->"),
    token_(ID     , 3, "int"),
    token_(LCBRK  , 1, "{"),
    token_(KEYWORD, 2, "if"),
    token_(LT     , 1, "<"),
    token_(GT     , 1, ">"),
    token_(INTLIT , 1, "1"),
    token_(KEYWORD, 2, "if"),
    token_(EOF_   , 0, ""),
  };
  munit_assert_int(sizeof(expected_tokens) / sizeof(token), ==, EXPECTED_TOKENS_COUNT);

  const char* content = "main =:,*->int{if<>1if";
  result_tokenizer_create t = tokenizer_create_cstr(content);
  munit_assert_true(t.isok);
  munit_assert_false(t.isfail);
  munit_logf(MUNIT_LOG_INFO, "Created tokenizer%s\n", "");

  result_tokenizer_run r = tokenizer_run(&t.ok);
  munit_logf(MUNIT_LOG_INFO, "Ran tokenizer%s\n", "");
  munit_assert_true(t.isok);

  munit_assert_int(EXPECTED_TOKENS_COUNT, ==, t.ok.tokens.size);

  for (int i = 0; i < t.ok.tokens.size; i++) {
    token actual = t.ok.tokens.buffer[i];
    token expected = expected_tokens[i];

    // assert each field of the token
    munit_assert_int(actual.type, ==, expected.type);
    munit_assert_int(actual.len, ==, expected.len);
    munit_assert_string_n_equal(actual.start, expected.start, actual.len);
    munit_logf(MUNIT_LOG_INFO, "Checked string %d%s\n", i, "");
  }

  tokenizer_free(&t.ok);
  munit_logf(MUNIT_LOG_INFO, "Freed tokenizer%s\n", "");
  return MUNIT_OK;
}
