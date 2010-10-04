set(CURR_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(SLHAEA_H ${CMAKE_SOURCE_DIR}/slhaea.h)

add_custom_command(
  OUTPUT  ${CURR_SRC_DIR}/ca-clang.txt
  COMMAND clang ARGS --analyze -x c++ ${SLHAEA_H}
            > ${CURR_SRC_DIR}/ca-clang.txt 2>&1
  DEPENDS ${SLHAEA_H})

add_custom_command(
  OUTPUT  ${CURR_SRC_DIR}/ca-cppcheck.txt
  COMMAND cppcheck ARGS -q --enable=all -I${CMAKE_SOURCE_DIR}
            ${CURR_SRC_DIR}/dummy.cpp > ${CURR_SRC_DIR}/ca-cppcheck.txt 2>&1
  DEPENDS ${SLHAEA_H})

add_custom_command(
  OUTPUT  ${CURR_SRC_DIR}/ca-flawfinder.txt
  COMMAND flawfinder ARGS -DQ ${SLHAEA_H}
            > ${CURR_SRC_DIR}/ca-flawfinder.txt 2>&1
  DEPENDS ${SLHAEA_H})

add_custom_command(
  OUTPUT  ${CURR_SRC_DIR}/ca-gcc-effc++.txt
  COMMAND g++ ARGS -Weffc++ -I${CMAKE_SOURCE_DIR} ${CURR_SRC_DIR}/dummy.cpp
            > ${CURR_SRC_DIR}/ca-gcc-effc++.txt 2>&1
  DEPENDS ${SLHAEA_H})

add_custom_target(ca-all
  DEPENDS ca-clang.txt
          ca-cppcheck.txt
          ca-flawfinder.txt
          ca-gcc-effc++.txt)
