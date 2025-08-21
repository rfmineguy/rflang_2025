cmake_minimum_required(VERSION 3.20)
project(rfc LANGUAGES C)
project(prebuild LANGUAGES C)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(PROFILING    "Turn on or off profiling" OFF)

# ================
# Setup variables for commonly used
#   directories
# ================
set(MAIN_SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")
set(EXERFC_SOURCE_DIR "${MAIN_SOURCE_DIR}/rfc")
set(LIBRFC_SOURCE_DIR "${MAIN_SOURCE_DIR}/librfc")
set(LIBRFC_THIRDPARTY_DIR "${MAIN_SOURCE_DIR}/thirdparty")
set(LIBRFC_TPL_SUPPORT_DIR "${MAIN_SOURCE_DIR}/tpl_support")
file(MAKE_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/generated")

# ================
# Setup prebuild
# ================
include(cmake/Prebuild.cmake)

set(GENGEN_TPL_FILES
  ${CMAKE_SOURCE_DIR}/tpl/stack.htpl
  ${CMAKE_SOURCE_DIR}/tpl/stack.ctpl
  ${CMAKE_SOURCE_DIR}/tpl/dynarray.htpl
  ${CMAKE_SOURCE_DIR}/tpl/dynarray.ctpl)

set(GENGEN_GENERATED_FILES
  ${LIBRFC_SOURCE_DIR}/stack_ast_node.h
  ${LIBRFC_SOURCE_DIR}/stack_ast_node.c
  ${LIBRFC_SOURCE_DIR}/dynarray_token.h
  ${LIBRFC_SOURCE_DIR}/dynarray_token.c)

prebuild_setup(
  "gengen.c"
  TARGET_NAME prebuildexe
  TPL_FILES "${GENGEN_TPL_FILES}"
  GENERATED_FILES "${GENGEN_GENERATED_FILES}"
)

# ==================================
# If profiling is requested add a few things to the build
# ==================================
set(RFC_COMPILE_OPTIONS "")
if (PROFILING)
  message("Profiling enabled")
  set(RFC_C_FLAGS -finstrument-functions -DENABLE_SPALL)
  list(APPEND LIBRFC_THIRDPARTY_FILES "${LIBRFC_THIRDPARTY_DIR}/instrument.c")
  list(APPEND LIBRFC_THIRDPARTY_FILES "${LIBRFC_THIRDPARTY_DIR}/spall_util.c")
endif()

# ==================================
# Setup the files to be compiled
# ==================================
list(APPEND LIBRFC_THIRDPARTY_FILES
  "${LIBRFC_THIRDPARTY_DIR}/arena_impl.c"
)

set(LIBRFC_SOURCE_FILES
  "${LIBRFC_SOURCE_DIR}/ast.c"
  "${LIBRFC_SOURCE_DIR}/ast_reconstruct.c"
  "${LIBRFC_SOURCE_DIR}/parser.c"
  "${LIBRFC_SOURCE_DIR}/token.c"
  "${LIBRFC_SOURCE_DIR}/tokenizer.c"
  "${LIBRFC_THIRDPARTY_FILES}"
  "${GENGEN_GENERATED_FILES}"
)

set(EXERFC_SOURCE_FILES
  "${EXERFC_SOURCE_DIR}/main.c"
)

# ==================================
# Setup librfc
# ==================================
add_library(librfc STATIC ${LIBRFC_SOURCE_FILES})
target_compile_options(librfc PRIVATE ${RFC_C_FLAGS})
add_dependencies(librfc prebuildexe)
target_include_directories(librfc PUBLIC
  "${LIBRFC_SOURCE_DIR}"
  "${LIBRFC_THIRDPARTY_DIR}"
  "${LIBRFC_TPL_SUPPORT_DIR}"
)

# ==================================
# Setup rfc driver executable
# ==================================
add_executable(rfc ${EXERFC_SOURCE_FILES})
target_compile_options(rfc PRIVATE ${RFC_C_FLAGS})
target_link_libraries(rfc PRIVATE librfc)
target_include_directories(rfc PUBLIC
  "${MAIN_SOURCE_DIR}"
  "${LIBRFC_SOURCE_DIR}"
  "${LIBRFC_THIRDPARTY_DIR}"
  "${LIBRFC_TPL_SUPPORT_DIR}"
)

add_subdirectory(tests)
