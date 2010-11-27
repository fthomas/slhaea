set(CURR_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(CURR_BIN_DIR ${CMAKE_CURRENT_BINARY_DIR})
string(REPLACE "/" "\\/" ESC_SRC_DIR ${CMAKE_SOURCE_DIR}/)

function(run_code_analyzer CMD ARGS OUTFILE)
    find_program(ANALYZER_${CMD} ${CMD})
    if(ANALYZER_${CMD})
        add_custom_command(
          OUTPUT  ${CURR_BIN_DIR}/${OUTFILE}
          COMMAND ${ANALYZER_${CMD}} ${ARGS} > ${OUTFILE} 2>&1
          COMMAND sed -i 's/${ESC_SRC_DIR}//' ${OUTFILE}
          COMMAND ${CMAKE_COPY} ${OUTFILE} ${CURR_SRC_DIR}/
          DEPENDS ${SLHAEA_H})
        set(CA_RESULTS ${CA_RESULTS};${CURR_BIN_DIR}/${OUTFILE} PARENT_SCOPE)
    endif()
endfunction()

set(clang_ARGS --analyze -x c++ ${SLHAEA_H})
run_code_analyzer(clang "${clang_ARGS}" clang.txt)

set(cppcheck_ARGS -q --enable=all -I${CMAKE_SOURCE_DIR}
  ${CURR_SRC_DIR}/dummy.cpp)
run_code_analyzer(cppcheck "${cppcheck_ARGS}" cppcheck.txt)

set(flawfinder_ARGS -DQ -m 0 ${SLHAEA_H})
run_code_analyzer(flawfinder "${flawfinder_ARGS}" flawfinder.txt)

set(g++_ARGS -std=c++0x -Weffc++ -I${CMAKE_SOURCE_DIR}
  ${CURR_SRC_DIR}/dummy.cpp)
run_code_analyzer(g++ "${g++_ARGS}" gcc-effc++.txt)

add_custom_target(code-analyses DEPENDS ${CA_RESULTS})
