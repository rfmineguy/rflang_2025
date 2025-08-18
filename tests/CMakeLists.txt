cmake_minimum_required(VERSION 3.20)
project(librfc-test LANGUAGES C)
include(CTest)
include(FetchContent)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

FetchContent_Declare(munit GIT_REPOSITORY https://github.com/nemequ/munit.git GIT_SHALLOW 1)
FetchContent_MakeAvailable(munit)

# ================
# Setup variables for commonly used
#   directories
# ================
set(MAIN_TEST_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/src")
set(LIB_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/lib")
file(MAKE_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/generated")

# ================
# Setup prebuild
# ================
include(../cmake/Prebuild.cmake)

set(GENGEN_TPL_FILES
  ${CMAKE_SOURCE_DIR}/tpl/stack.htpl
  ${CMAKE_SOURCE_DIR}/tpl/stack.ctpl
  ${CMAKE_SOURCE_DIR}/tpl/dynarray.htpl
  ${CMAKE_SOURCE_DIR}/tpl/dynarray.ctpl)

set(GENGEN_GENERATED_FILES
  ${CMAKE_CURRENT_LIST_DIR}/generated/stack_test_type.h
  ${CMAKE_CURRENT_LIST_DIR}/generated/stack_test_type.c
  ${CMAKE_CURRENT_LIST_DIR}/generated/dynarray_token.h
  ${CMAKE_CURRENT_LIST_DIR}/generated/dynarray_token.c)

prebuild_setup(
  "gengen.c"
  TARGET_NAME prebuildexe2
  TPL_FILES "${GENGEN_TPL_FILES}"
  GENERATED_FILES "${GENGEN_GENERATED_FILES}"
)

# ==================================
# Setup the files to be compiled
# ==================================
set(TEST_FILES
  "${MAIN_TEST_SOURCE_DIR}/dynarray_token.c"
  "${MAIN_TEST_SOURCE_DIR}/main.c"
  "${MAIN_TEST_SOURCE_DIR}/parser_run.c"
  "${MAIN_TEST_SOURCE_DIR}/parser_stack.c"
  "${MAIN_TEST_SOURCE_DIR}/result.c"
  "${MAIN_TEST_SOURCE_DIR}/stack_test_type.c"
  "${MAIN_TEST_SOURCE_DIR}/tests.h"
  "${MAIN_TEST_SOURCE_DIR}/token.c"
  "${MAIN_TEST_SOURCE_DIR}/tokenizer.c"
  "${MAIN_TEST_SOURCE_DIR}/variant.c"
  "${GENGEN_GENERATED_FILES}"
)

set(LIB_FILES
  "${munit_SOURCE_DIR}/munit.c"
)

message("${munit_SOURCE_DIR}")

add_executable(librfc-test ${TEST_FILES} ${LIB_FILES})
target_link_libraries(librfc-test PRIVATE librfc)
add_dependencies(librfc-test prebuildexe2)
target_include_directories(librfc-test PUBLIC support support/tpl lib/ generated ${munit_SOURCE_DIR})
target_compile_options(librfc-test PUBLIC "-DMUNIT_TEST_NAME_LEN=80")

add_test(
  NAME MyExecutableTest
  COMMAND librfc-test
  WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
)
