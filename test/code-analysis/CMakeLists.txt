set(CURR_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(SLHAEA_H ${CMAKE_SOURCE_DIR}/slhaea.h)

function(add_code_analyzer CMD ARGS OUTFILE)
    find_program(CODE_ANALYZER_${CMD} ${CMD})
    if(CODE_ANALYZER_${CMD})
        add_custom_command(
          OUTPUT  ${CURR_SRC_DIR}/${OUTFILE}
          COMMAND ${CODE_ANALYZER_${CMD}}
          ARGS    ${ARGS} > ${CURR_SRC_DIR}/${OUTFILE} 2>&1
          DEPENDS ${SLHAEA_H})

        set(CA_RESULTS ${CA_RESULTS};${OUTFILE} PARENT_SCOPE)
    endif()
endfunction()

add_code_analyzer(clang
  "--analyze;-x;c++;${SLHAEA_H}"
  ca-clang.txt)

add_code_analyzer(cppcheck
  "-q;--enable=all;-I${CMAKE_SOURCE_DIR};${CURR_SRC_DIR}/dummy.cpp"
  ca-cppcheck.txt)

add_code_analyzer(flawfinder
  "-DQ;-m;0;${SLHAEA_H}"
  ca-flawfinder.txt)

add_code_analyzer(g++
  "-Weffc++;-I${CMAKE_SOURCE_DIR};${CURR_SRC_DIR}/dummy.cpp"
  ca-gcc-effc++.txt)

string(REPLACE "/" "\\/" ESC_SRC_DIR ${CMAKE_SOURCE_DIR}/)
add_custom_target(ca-all DEPENDS ${CA_RESULTS}
  COMMAND sed -i 's/${ESC_SRC_DIR}//' ${CA_RESULTS}
  WORKING_DIRECTORY ${CURR_SRC_DIR})
