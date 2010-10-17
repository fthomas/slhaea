set(CURR_DIR ${CMAKE_CURRENT_SOURCE_DIR})

function(run_gprof BINARY OUTFILE)
    find_program(PROFILER_gprof gprof)
    if(PROFILER_gprof)
        get_target_property(BIN_LOC ${BINARY} LOCATION)
        add_custom_command(
          OUTPUT  ${CURR_DIR}/${OUTFILE}
          COMMAND ${BIN_LOC}
          COMMAND ${PROFILER_gprof}
          ARGS    -b ${BIN_LOC} > ${CURR_DIR}/${OUTFILE} 2>&1
          DEPENDS ${SLHAEA_H} ${BINARY})
        set(GPROF_RESULTS ${GPROF_RESULTS};${OUTFILE} PARENT_SCOPE)
    endif()
endfunction()

function(run_valgrind TOOL BINARY OUTFILE)
    find_program(PROFILER_valg valgrind)
    if(PROFILER_valg)
        get_target_property(BIN_LOC ${BINARY} LOCATION)
        add_custom_command(
          OUTPUT  ${CURR_DIR}/${OUTFILE}
          COMMAND ${PROFILER_valg}
          ARGS    --tool=${TOOL} ${BIN_LOC} > ${CURR_DIR}/${OUTFILE} 2>&1
          DEPENDS ${SLHAEA_H} ${BINARY})
        set(VALG_RESULTS ${VALG_RESULTS};${OUTFILE} PARENT_SCOPE)
    endif()
endfunction()

configure_file(input.txt . COPYONLY)

include_directories(${CMAKE_SOURCE_DIR} ${Boost_INCLUDE_DIRS})

add_executable(input  input.cpp  ${SLHAEA_H})
add_executable(output output.cpp ${SLHAEA_H})
set_target_properties(input output PROPERTIES COMPILE_FLAGS "-g -O2")

if(CMAKE_COMPILER_IS_GNUCXX)
    add_executable(input-pg  input.cpp  ${SLHAEA_H})
    add_executable(output-pg output.cpp ${SLHAEA_H})
    set_target_properties(input-pg output-pg PROPERTIES
      COMPILE_FLAGS "-pg -g -O2"
      LINK_FLAGS    "-pg")

    run_gprof(input-pg  gprof-input.txt)
    run_gprof(output-pg gprof-output.txt)
endif()

run_valgrind(callgrind input  callgrind-input.txt)
run_valgrind(callgrind output callgrind-output.txt)
run_valgrind(callgrind ut     callgrind-ut.txt)

run_valgrind(memcheck input  memcheck-input.txt)
run_valgrind(memcheck output memcheck-output.txt)
run_valgrind(memcheck ut     memcheck-ut.txt)

string(REPLACE "/" "\\/" ESC_BIN_DIR ${CMAKE_BINARY_DIR}/)
add_custom_target(profiles
  DEPENDS ${GPROF_RESULTS} ${VALG_RESULTS}
  COMMAND sed -i 's/==[0-9]\\+==[ ]*//' ${VALG_RESULTS}
  COMMAND sed -i 's/${ESC_BIN_DIR}//' ${VALG_RESULTS}
  WORKING_DIRECTORY ${CURR_DIR})
