#include "tests.h"
#include <stdbool.h>
#include "../../src/librfc/variant.h"

#define datatype_variant(X)\
  X(Int,    datatype, int)\
  X(String, datatype, const char*)\
  X(Float,  datatype, float)\

define_variant(datatype, datatype_variant);

MunitResult variant_datatype_test(const MunitParameter *params, void *context) {
  variant_datatype v = make_variant(datatype, Int, 3);
  munit_assert_int(v.type, ==, variant_datatype_type_Int);
  munit_assert_int(v.Int, ==, 3);

  v = make_variant(datatype, String, "Hello world");
  munit_assert_int(v.type, ==, variant_datatype_type_String);
  munit_assert_string_equal(v.String, "Hello world");

  v = make_variant(datatype, Float, 0.531f);
  munit_assert_int(v.type, ==, variant_datatype_type_Float);
  munit_assert_float(v.Float, ==, 0.531f);
  return MUNIT_OK;
}

MunitResult variant_datatype_Int_match_test(const MunitParameter *params, void *context) {
  variant_datatype variant = make_variant(datatype, Int, 3);
  munit_assert_int(variant.type, ==, variant_datatype_type_Int);
  match_variant(variant, datatype,
    variant_case(datatype, Int,    { munit_assert_int(v.Int, ==, 3); })
    variant_case(datatype, Float,  { munit_assert(false); })
    variant_case(datatype, String, { munit_assert(false); })
    variant_default({ munit_assert(false); /* unreachable */ })
  )
  return MUNIT_OK;
}

MunitResult variant_datatype_Float_match_test(const MunitParameter *params, void *context) {
  variant_datatype variant = make_variant(datatype, Float, 3.3);
  match_variant(variant, datatype,
    variant_case(datatype, Int,    { munit_assert(false); })
    variant_case(datatype, Float,  { munit_assert_float(v.Float, ==, 3.3); })
    variant_case(datatype, String, { munit_assert(false); })
    variant_default({ munit_assert(false); /* unreachable */ })
  )
  return MUNIT_OK;
}

MunitResult variant_datatype_String_match_test(const MunitParameter *params, void *context) {
  variant_datatype variant = make_variant(datatype, String, "hello");
  match_variant(variant, datatype,
    variant_case(datatype, Int,    { munit_assert(false); })
    variant_case(datatype, Float,  { munit_assert(false); })
    variant_case(datatype, String, { munit_assert_string_equal(v.String, "hello"); })
    variant_default({ munit_assert(false); /* unreachable */ })
  )
  return MUNIT_OK;
}
