add_subdirectory(examples)
add_subdirectory(manual)

if(DOXYGEN_FOUND)
    configure_file(Doxyfile.in Doxyfile)
    add_custom_target(doxygen
      ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
      SOURCES Doxyfile.in)
endif()
