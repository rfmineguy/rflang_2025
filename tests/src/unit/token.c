#include "tests.h"
#include "../../src/librfc/token.h"

MunitResult token_type_str_test(const MunitParameter *params, void *context) {
  munit_assert_string_equal(token_type_str(ID), "ID");
  munit_assert_string_equal(token_type_str(EQ), "EQ");
  munit_assert_string_equal(token_type_str(PLUS), "PLUS");
  munit_assert_string_equal(token_type_str(MINUS), "MINUS");
  munit_assert_string_equal(token_type_str(MUL), "MUL");
  munit_assert_string_equal(token_type_str(DIV), "DIV");
  munit_assert_string_equal(token_type_str(MOD), "MOD");
  munit_assert_string_equal(token_type_str(COLON), "COLON");
  munit_assert_string_equal(token_type_str(SEMI), "SEMI");
  munit_assert_string_equal(token_type_str(COMMA), "COMMA");
  munit_assert_string_equal(token_type_str(SQUOT), "SQUOT");
  munit_assert_string_equal(token_type_str(DQUOT), "DQUOT");
  munit_assert_string_equal(token_type_str(ARROW), "ARROW");
  munit_assert_string_equal(token_type_str(KEYWORD), "KEYWORD");
  munit_assert_string_equal(token_type_str(LCBRK), "LCBRK");

  munit_assert_string_equal(token_type_str(RCBRK), "RCBRK");
  munit_assert_string_equal(token_type_str(LSBRK), "LSBRK");
  munit_assert_string_equal(token_type_str(RSBRK), "RSBRK");
  munit_assert_string_equal(token_type_str(LPAR), "LPAR");
  munit_assert_string_equal(token_type_str(RPAR), "RPAR");

  munit_assert_string_equal(token_type_str(LT), "LT");
  munit_assert_string_equal(token_type_str(LTEQ), "LTEQ");
  munit_assert_string_equal(token_type_str(GT), "GT");
  munit_assert_string_equal(token_type_str(GTEQ), "GTEQ");

  munit_assert_string_equal(token_type_str(INTLIT), "INTLIT");
  munit_assert_string_equal(token_type_str(STRLIT), "STRLIT");

  munit_assert_string_equal(token_type_str(EOF_), "EOF_");

  munit_assert_string_equal(token_type_str(DEQ), "DEQ");
  munit_assert_string_equal(token_type_str(DAND),"DAND");
  munit_assert_string_equal(token_type_str(DOR), "DOR");

  munit_assert_string_equal(token_type_str(KEYWORD), "KEYWORD");

  for (int i = TOKEN_TYPE_LAST; i < TOKEN_TYPE_LAST + 50; i++)
    munit_assert_string_equal(token_type_str(i), "<UNDEFINED TOKEN TYPE>");

  return MUNIT_OK;
}
