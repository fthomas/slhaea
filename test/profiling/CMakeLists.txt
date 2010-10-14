set(CURR_DIR ${CMAKE_CURRENT_SOURCE_DIR})

configure_file(input.txt . COPYONLY)

include_directories(${CMAKE_SOURCE_DIR} ${Boost_INCLUDE_DIRS})

add_executable(input  input.cpp  ${SLHAEA_H})
add_executable(output output.cpp ${SLHAEA_H})
set_target_properties(input output PROPERTIES COMPILE_FLAGS "-O2")

if(CMAKE_COMPILER_IS_GNUCXX)
    add_executable(input-pg  input.cpp  ${SLHAEA_H})
    add_executable(output-pg output.cpp ${SLHAEA_H})
    set_target_properties(input-pg output-pg PROPERTIES
      COMPILE_FLAGS "-pg -O2"
      LINK_FLAGS    "-pg")
endif()

macro(add_profiler CMD ARGS OUTFILE DEPENDENCIES)
    add_analyzer(${CMD} "${ARGS}" ${CURR_DIR}/${OUTFILE} ${DEPENDENCIES})
    if(ANALYZER_${CMD})
        list(APPEND PROF_RESULTS ${OUTFILE})
    endif()
endmacro()

set(callgrind_in_ARGS --tool=callgrind ./input)
add_profiler(valgrind "${callgrind_in_ARGS}" callgrind-input.txt input)

set(callgrind_out_ARGS --tool=callgrind ./output)
add_profiler(valgrind "${callgrind_out_ARGS}" callgrind-output.txt output)

set(memcheck_in_ARGS --tool=memcheck --leak-check=full ./input)
add_profiler(valgrind "${memcheck_in_ARGS}" memcheck-input.txt input)

set(memcheck_out_ARGS --tool=memcheck --leak-check=full ./output)
add_profiler(valgrind "${memcheck_out_ARGS}" memcheck-output.txt output)

add_custom_target(profile-all DEPENDS ${PROF_RESULTS}
  COMMAND sed -i 's/==[0-9]\+==[ ]*//' ${PROF_RESULTS}
  WORKING_DIRECTORY ${CURR_DIR})
