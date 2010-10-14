configure_file(input.txt . COPYONLY)

include_directories(${CMAKE_SOURCE_DIR} ${Boost_INCLUDE_DIRS})

add_executable(input  input.cpp  ${SLHAEA_H})
add_executable(output output.cpp ${SLHAEA_H})

set(inout_COMPILE_FLAGS "-O2")
set(inout_LINK_FLAGS    "")

if(CMAKE_COMPILER_IS_GNUCXX)
    set(inout_COMPILE_FLAGS "${inout_COMPILE_FLAGS} -pg")
    set(inout_LINK_FLAGS    "${inout_LINK_FLAGS} -pg")
endif()

set_target_properties(input output PROPERTIES
  COMPILE_FLAGS "${inout_COMPILE_FLAGS}"
  LINK_FLAGS    "${inout_LINK_FLAGS}")

find_program(PROFILER_GPROF gprof)
find_program(PROFILER_VALGRIND valgrind)
