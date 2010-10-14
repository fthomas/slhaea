function(add_analyzer CMD ARGS OUTFILE)
    find_program(ANALYZER_${CMD} ${CMD})
    if(ANALYZER_${CMD})
        add_custom_command(
          OUTPUT  ${OUTFILE}
          COMMAND ${ANALYZER_${CMD}}
          ARGS    ${ARGS} > ${OUTFILE} 2>&1
          DEPENDS ${SLHAEA_H})
    endif()
endfunction()

add_subdirectory(code-analysis)
add_subdirectory(profiling)
add_subdirectory(unit-tests)
