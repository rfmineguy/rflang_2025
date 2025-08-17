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
      munit_errorf("assertion failed: string \n\'%s\' != \'%s\' (\"%s\" == \"%s\")", \
                   #a, #b, munit_tmp_a_, munit_tmp_b_); \
    } \
    MUNIT_PUSH_DISABLE_MSVC_C4127_ \
  } while (0) \
  MUNIT_POP_DISABLE_MSVC_C4127_


/*
 * 
 */
#define munit_assert_file_contents_equal(filename, contents) \
  do {\
    FILE* f = fopen(filename, "r");\
    if (!f) munit_errorf("assertion failed: filename '%s' couldn't be opened", filename);\
    fseek(f, 0, SEEK_END);\
    size_t file_size = ftell(f);\
    fseek(f, 0, SEEK_SET);\
    char* content = (char*)malloc(file_size + 1);\
    if (!content) {\
      fclose(f);\
      munit_errorf("assertion failed: failed to alloc memory for file read%s", "");\
    }\
    fread(content, 1, file_size, f);\
    content[file_size] = 0;\
    munit_assert_string_equal(content, contents);\
    MUNIT_PUSH_DISABLE_MSVC_C4127_ \
  } while (0)\
  MUNIT_POP_DISABLE_MSVC_C4127_
