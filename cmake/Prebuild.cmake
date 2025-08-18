# +----------------------------------------------
# | 1. Setup the prebuild target
# +----------------------------------------------
# |  include(cmake/Prebuild.cmake)
# |
# |  set(GENGEN_TPL_FILES
# |    ${CMAKE_SOURCE_DIR}/tpl/stack.htpl
# |    ${CMAKE_SOURCE_DIR}/tpl/stack.ctpl
# |    ${CMAKE_SOURCE_DIR}/tpl/dynarray.htpl
# |    ${CMAKE_SOURCE_DIR}/tpl/dynarray.ctpl)
# |
# |  set(GENGEN_GENERATED_FILES
# |    ${LIBRFC_SOURCE_DIR}/stack_ast_node.h
# |    ${LIBRFC_SOURCE_DIR}/stack_ast_node.c
# |    ${LIBRFC_SOURCE_DIR}/dynarray_token.h
# |    ${LIBRFC_SOURCE_DIR}/dynarray_token.c)
# |
# |  prebuild_setup(
# |    "gengen.c"
# |    TARGET_NAME prebuildexe
# |    TPL_FILES "${GENGEN_TPL_FILES}"
# |    GENERATED_FILES "${GENGEN_GENERATED_FILES}"
# |  )
# +----------------------------------------------
# | 2. Depend on this prebuild target
# +----------------------------------------------
# |  add_library(name STATIC files)
# |  add_dependencies(name prebuildexe)
# +----------------------------------------------
function(prebuild_setup GEN)
  set(options "")
  set(oneValueArgs TARGET_NAME)
  set(multiValueArgs TPL_FILES GENERATED_FILES)

  cmake_parse_arguments(PREBUILD "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  add_executable(${PREBUILD_TARGET_NAME} "${GEN}")
  add_custom_command(
    OUTPUT ${PREBUILD_GENERATED_FILES}
    DEPENDS ${PREBUILD_TARGET_NAME} ${PREBUILD_TPL_FILES}
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    COMMAND $<TARGET_FILE:${PREBUILD_TARGET_NAME}>
    COMMENT "Running prebuild built from ${GEN}"
  )
endfunction()
