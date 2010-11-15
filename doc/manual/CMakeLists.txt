set(CURR_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(CURR_BIN_DIR ${CMAKE_CURRENT_BINARY_DIR})

if(PDFLATEX_COMPILER AND BIBTEX_COMPILER)
    configure_file(slhaea.tex.in slhaea.tex)
    configure_file(slhaea.bib . COPYONLY)
    add_custom_command(OUTPUT ${CURR_BIN_DIR}/slhaea.pdf
      COMMAND ${PDFLATEX_COMPILER} slhaea
      COMMAND ${BIBTEX_COMPILER}   slhaea
      COMMAND ${PDFLATEX_COMPILER} slhaea
      COMMAND ${PDFLATEX_COMPILER} slhaea
      COMMAND ${CMAKE_COPY} slhaea.pdf ${CURR_SRC_DIR}/
      DEPENDS ${CURR_SRC_DIR}/slhaea.tex.in
              ${CURR_SRC_DIR}/slhaea.bib)

    add_custom_target(manual DEPENDS ${CURR_BIN_DIR}/slhaea.pdf
      SOURCES slhaea.tex.in slhaea.bib)
endif()
