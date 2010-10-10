include_directories(${CMAKE_SOURCE_DIR} ${Boost_INCLUDE_DIRS})

file(GLOB UT_SOURCES *.cpp *.h)
add_executable(ut ${UT_SOURCES} ${SLHAEA_H})
target_link_libraries(ut ${Boost_LIBRARIES})

if(CMAKE_COMPILER_IS_GNUCXX)
    set_target_properties(ut PROPERTIES
      COMPILE_FLAGS "--coverage -g -O0"
      LINK_FLAGS    "--coverage")

    find_program(COVERAGE_LCOV lcov)
    find_program(COVERAGE_GENHTML genhtml)
    if(COVERAGE_LCOV AND COVERAGE_GENHTML)
        set(COVERAGE_OUTFILE ${CMAKE_CURRENT_BINARY_DIR}/coverage.info)
        add_custom_target(lcov
          COMMAND ut
          COMMAND ${COVERAGE_LCOV}
            --capture
            --directory ${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/ut.dir/
            --base-directory ${CMAKE_SOURCE_DIR}
            --output-file ${COVERAGE_OUTFILE}
          COMMAND ${COVERAGE_LCOV}
            --zerocounters
            --directory ${CMAKE_CURRENT_BINARY_DIR}
          COMMAND ${COVERAGE_LCOV}
            --extract ${COVERAGE_OUTFILE} "\\*slhaea.h"
            --output-file ${COVERAGE_OUTFILE}
          COMMAND ${COVERAGE_GENHTML}
            --output-directory ${CMAKE_CURRENT_SOURCE_DIR}/lcov-report
            --prefix ${CMAKE_SOURCE_DIR}
            --legend --demangle-cpp --title "SLHAea"
            ${COVERAGE_OUTFILE})
    endif()
endif()

add_test(NAME all_unit_tests COMMAND ut)
