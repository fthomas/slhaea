set(CURR_DIR ${CMAKE_CURRENT_SOURCE_DIR})

function(run_code_analyzer CMD ARGS OUTFILE)
    find_program(ANALYZER_${CMD} ${CMD})
    if(ANALYZER_${CMD})
        add_custom_command(
          OUTPUT  ${CURR_DIR}/${OUTFILE}
          COMMAND ${ANALYZER_${CMD}}
          ARGS    ${ARGS} > ${CURR_DIR}/${OUTFILE} 2>&1
          DEPENDS ${SLHAEA_H})
        set(CA_RESULTS ${CA_RESULTS};${OUTFILE} PARENT_SCOPE)
    endif()
endfunction()

set(clang_ARGS --analyze -x c++ ${SLHAEA_H})
run_code_analyzer(clang "${clang_ARGS}" clang.txt)

set(cppcheck_ARGS -q --enable=all -I${CMAKE_SOURCE_DIR} ${CURR_DIR}/dummy.cpp)
run_code_analyzer(cppcheck "${cppcheck_ARGS}" cppcheck.txt)

set(flawfinder_ARGS -DQ -m 0 ${SLHAEA_H})
run_code_analyzer(flawfinder "${flawfinder_ARGS}" flawfinder.txt)

set(g++_ARGS -Weffc++ -I${CMAKE_SOURCE_DIR} ${CURR_DIR}/dummy.cpp)
run_code_analyzer(g++ "${g++_ARGS}" gcc-effc++.txt)

string(REPLACE "/" "\\/" ESC_SRC_DIR ${CMAKE_SOURCE_DIR}/)
add_custom_target(code-analyses
  DEPENDS ${CA_RESULTS}
  COMMAND sed -i 's/${ESC_SRC_DIR}//' ${CA_RESULTS}
  WORKING_DIRECTORY ${CURR_DIR})
