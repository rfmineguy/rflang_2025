function(gengetopt_setup CONFIG_GGO)
  set(options "")
  set(oneValueArgs OUTPUT_DIR)
  set(multiValueArgs CMDLINE_FILES)

  cmake_parse_arguments(PREBUILD "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  add_custom_command(
    OUTPUT ${PREBUILD_CMDLINE_FILES}
    DEPENDS ${CONFIG_GGO}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMAND gengetopt --input=${CONFIG_GGO} --output-dir=${PREBUILD_OUTPUT_DIR}
    COMMENT "Running gengetopt"
  )
endfunction()
