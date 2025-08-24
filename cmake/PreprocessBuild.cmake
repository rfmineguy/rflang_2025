function(preprocess_build)
  set(options "")
  set(oneValueArgs MODIFY)
  set(multiValueArgs SOURCE_FILES INCLUDE_DIRS CFLAGS)

  cmake_parse_arguments(PREBUILD "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  # Ensure output directory exists
  file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/preprocessed)

  # Convert include dirs to flags
  set(INCLUDE_FLAGS)
  foreach(dir IN LISTS PREBUILD_INCLUDE_DIRS)
      list(APPEND INCLUDE_FLAGS "-I${dir}")
  endforeach()

  set(PREPROCESS_FILES)
  foreach (src ${PREBUILD_SOURCE_FILES})
    if(src MATCHES "\\.c$")
      get_filename_component(name ${src} NAME_WE)
      set(pp_file "${CMAKE_BINARY_DIR}/preprocessed/${name}.i")
      list(APPEND PREPROCESS_FILES ${pp_file})

      add_custom_command(
        OUTPUT ${pp_file}
        COMMAND ${CMAKE_C_COMPILER} ${PREBUILD_CFLAGS} -E -P ${src} -o ${pp_file} ${INCLUDE_FLAGS} | clang-format > ${pp_file}
        DEPENDS ${src}
        COMMENT "Preprocessing ${src}"
      )
    endif()
  endforeach()

  set(${PREBUILD_MODIFY} ${PREPROCESS_FILES} PARENT_SCOPE)
endfunction()
