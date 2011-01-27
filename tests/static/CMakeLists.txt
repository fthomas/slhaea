set(CURR_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(CURR_BIN_DIR ${CMAKE_CURRENT_BINARY_DIR})
string(REPLACE "/" "\\/" ESC_SRC_DIR ${CMAKE_SOURCE_DIR}/)

function(run_static_analyzer CMD ARGS OUTFILE)
    find_program(ANALYZER_${CMD} ${CMD})
    if(ANALYZER_${CMD})
        add_custom_command(
          OUTPUT  ${OUTFILE}
          COMMAND ${ANALYZER_${CMD}} ${ARGS} > ${OUTFILE} 2>&1 || true
          COMMAND sed -i 's/${ESC_SRC_DIR}//' ${OUTFILE}
          COMMAND ${CMAKE_COPY} ${OUTFILE} ${CURR_SRC_DIR}/
          DEPENDS ${SLHAEA_H})
        set(RESULTS ${RESULTS};${CURR_BIN_DIR}/${OUTFILE} PARENT_SCOPE)
    endif()
endfunction()

set(clang_ARGS --analyze -x c++ ${SLHAEA_H})
run_static_analyzer(clang "${clang_ARGS}" sa-clang.txt)

set(cppcheck_ARGS --quiet --enable=all -I${CMAKE_SOURCE_DIR}
  ${CURR_SRC_DIR}/dummy.cpp)
run_static_analyzer(cppcheck "${cppcheck_ARGS}" sa-cppcheck.txt)

set(flawfinder_ARGS --dataonly --quiet --minlevel=0 ${SLHAEA_H})
run_static_analyzer(flawfinder "${flawfinder_ARGS}" sa-flawfinder.txt)

set(g++_ARGS -S -Weffc++ -I${CMAKE_SOURCE_DIR} ${CURR_SRC_DIR}/dummy.cpp)
run_static_analyzer(g++ "${g++_ARGS}" sa-g++-effc++.txt)

set(icpc_ARGS -S -Weffc++ -wd2012 -wd2015 -I${CMAKE_SOURCE_DIR}
  ${CURR_SRC_DIR}/dummy.cpp)
run_static_analyzer(icpc "${icpc_ARGS}" sa-icpc-effc++.txt)


add_custom_target(static-analyses DEPENDS ${RESULTS})
