set(CURR_DIR ${CMAKE_CURRENT_SOURCE_DIR})

macro(add_code_analyzer CMD ARGS OUTFILE)
    add_analyzer(${CMD} "${ARGS}" ${CURR_DIR}/${OUTFILE} "")
    if(ANALYZER_${CMD})
        list(APPEND CA_RESULTS ${OUTFILE})
    endif()
endmacro()

set(clang_ARGS --analyze -x c++ ${SLHAEA_H})
add_code_analyzer(clang "${clang_ARGS}" clang.txt)

set(cppcheck_ARGS -q --enable=all -I${CMAKE_SOURCE_DIR} ${CURR_DIR}/dummy.cpp)
add_code_analyzer(cppcheck "${cppcheck_ARGS}" cppcheck.txt)

set(flawfinder_ARGS -DQ -m 0 ${SLHAEA_H})
add_code_analyzer(flawfinder "${flawfinder_ARGS}" flawfinder.txt)

set(g++_ARGS -Weffc++ -I${CMAKE_SOURCE_DIR} ${CURR_DIR}/dummy.cpp)
add_code_analyzer(g++ "${g++_ARGS}" gcc-effc++.txt)

string(REPLACE "/" "\\/" ESC_SRC_DIR ${CMAKE_SOURCE_DIR}/)
add_custom_target(ca-all DEPENDS ${CA_RESULTS}
  COMMAND sed -i 's/${ESC_SRC_DIR}//' ${CA_RESULTS}
  WORKING_DIRECTORY ${CURR_DIR})
