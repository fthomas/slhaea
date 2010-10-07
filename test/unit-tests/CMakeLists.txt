include_directories(${CMAKE_SOURCE_DIR} ${Boost_INCLUDE_DIRS})

file(GLOB UT_SOURCES ut-*.cpp ut-*.h)
add_executable(ut ${UT_SOURCES} ${CMAKE_SOURCE_DIR}/slhaea.h)
target_link_libraries(ut ${Boost_LIBRARIES})

if(CMAKE_COMPILER_IS_GNUCXX)
    set_target_properties(ut PROPERTIES
      COMPILE_FLAGS "--coverage -g -O0"
      LINK_FLAGS    "--coverage")
endif()

add_test(NAME all_unit_tests COMMAND ut)
