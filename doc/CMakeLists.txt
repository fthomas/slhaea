if(DOXYGEN_FOUND)
    add_custom_target(doxygen ${DOXYGEN_EXECUTABLE} Doxyfile
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
      SOURCES Doxyfile)
endif()
