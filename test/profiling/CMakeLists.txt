include_directories(${CMAKE_SOURCE_DIR} ${Boost_INCLUDE_DIRS})

add_executable(input  input.cpp  ${SLHAEA_H})
add_executable(output output.cpp ${SLHAEA_H})

configure_file(input.txt . COPYONLY)

find_program(PROFILER_GPROF gprof)
find_program(PROFILER_VALGRIND valgrind)
