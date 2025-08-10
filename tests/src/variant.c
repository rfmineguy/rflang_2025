#include "tests.h"
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
  return MUNIT_OK;
}
