set(CURR_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(CURR_BIN_DIR ${CMAKE_CURRENT_BINARY_DIR})
string(REPLACE "/" "\\/" ESC_BIN_DIR ${CMAKE_BINARY_DIR}/)

function(run_gprof BINARY OUTFILE)
    find_program(PROFILER_gprof gprof)
    if(PROFILER_gprof)
        add_custom_command(
          OUTPUT  ${CURR_BIN_DIR}/${OUTFILE}
          COMMAND $<TARGET_FILE:${BINARY}>
          COMMAND ${PROFILER_gprof} -b $<TARGET_FILE:${BINARY}> > ${OUTFILE} 2>&1
          COMMAND ${CMAKE_COPY} ${OUTFILE} ${CURR_SRC_DIR}/
          DEPENDS ${SLHAEA_H} ${BINARY})
        set(GPROF_RESULTS ${GPROF_RESULTS};${CURR_BIN_DIR}/${OUTFILE}
          PARENT_SCOPE)
    endif()
endfunction()

function(run_valgrind TOOL BINARY OUTFILE)
    find_program(PROFILER_valg valgrind)
    if(PROFILER_valg)
        add_custom_command(
          OUTPUT  ${CURR_BIN_DIR}/${OUTFILE}
          COMMAND ${PROFILER_valg} --tool=${TOOL} $<TARGET_FILE:${BINARY}> > ${OUTFILE} 2>&1
          COMMAND sed -i 's/==[0-9]\\+==[ ]*//' ${OUTFILE}
          COMMAND sed -i 's/${ESC_BIN_DIR}//' ${OUTFILE}
          COMMAND ${CMAKE_COPY} ${OUTFILE} ${CURR_SRC_DIR}/
          DEPENDS ${SLHAEA_H} ${BINARY})
        set(VALG_RESULTS ${VALG_RESULTS};${CURR_BIN_DIR}/${OUTFILE}
          PARENT_SCOPE)
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

add_custom_target(profiles DEPENDS ${GPROF_RESULTS} ${VALG_RESULTS})
