set(CURR_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(SLHAEA_H ${CMAKE_SOURCE_DIR}/slhaea.h)

function(add_ca_command CMD ARGS RESULT)
    find_program(${CMD}_EXEC ${CMD})
    if(${CMD}_EXEC)
        add_custom_command(
          OUTPUT  ${CURR_SRC_DIR}/${RESULT}
          COMMAND ${${CMD}_EXEC} ARGS ${ARGS} > ${CURR_SRC_DIR}/${RESULT} 2>&1
          DEPENDS ${SLHAEA_H})

        set(CA_RESULTS ${CA_RESULTS};${RESULT} PARENT_SCOPE)
    endif()
endfunction()

add_ca_command(clang
  "--analyze;-x;c++;${SLHAEA_H}"
  ca-clang.txt)

add_ca_command(cppcheck
  "-q;--enable=all;-I${CMAKE_SOURCE_DIR};${CURR_SRC_DIR}/dummy.cpp"
  ca-cppcheck.txt)

add_ca_command(flawfinder
  "-DQ;${SLHAEA_H}"
  ca-flawfinder.txt)

add_ca_command(g++
  "-Weffc++;-I${CMAKE_SOURCE_DIR};${CURR_SRC_DIR}/dummy.cpp"
  ca-gcc-effc++.txt)

string(REPLACE "/" "\\/" ESC_SRC_DIR ${CMAKE_SOURCE_DIR}/)
add_custom_target(ca-all DEPENDS ${CA_RESULTS}
  COMMAND sed -i 's/${ESC_SRC_DIR}//' ${CA_RESULTS}
  WORKING_DIRECTORY ${CURR_SRC_DIR})
