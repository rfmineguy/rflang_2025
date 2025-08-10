#include "munit.h"

/*
 * This macro was written by me (Riley Fischer) to extend the munit suite to
 * include a string equal assertion that can work for non null terminated 
 * strings with known lenth
 */
#define munit_assert_string_n_equal(a, b, n) \
  do { \
    const char* munit_tmp_a_ = a;\
    const char* munit_tmp_b_ = b;\
    if (MUNIT_UNLIKELY(strncmp(munit_tmp_a_, munit_tmp_b_, n) != 0)) {\
      munit_errorf("assertion failed: string %s == %s (\"%s\" == \"%s\")", \
                   #a, #b, munit_tmp_a_, munit_tmp_b_); \
    } \
    MUNIT_PUSH_DISABLE_MSVC_C4127_ \
  } while (0) \
  MUNIT_POP_DISABLE_MSVC_C4127_


/*
 * This macro was written by me (Riley Fischer) to extend the munit suite to
 * include a string not equal assertion that can work for non null terminated 
 * strings with known lenth
 */
#define munit_assert_string_n_not_equal(a, b, n) \
  do { \
    const char* munit_tmp_a_ = a;\
    const char* munit_tmp_b_ = b;\
    if (MUNIT_UNLIKELY(strncmp(munit_tmp_a, munit_tmp_b_, n) == 0)) {\
      munit_errorf("assertion failed: string %s != %s (\"%s\" == \"%s\")", \
                   #a, #b, munit_tmp_a_, munit_tmp_b_); \
    } \
    MUNIT_PUSH_DISABLE_MSVC_C4127_ \
  } while (0) \
  MUNIT_POP_DISABLE_MSVC_C4127_

