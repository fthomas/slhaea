include_directories(${CMAKE_SOURCE_DIR} ${Boost_INCLUDE_DIRS})

file(GLOB UT_SOURCES ut-*.cpp ut-*.h)
add_executable(ut ${UT_SOURCES} ${CMAKE_SOURCE_DIR}/slhaea.h)
target_link_libraries(ut ${Boost_LIBRARIES})

add_test(NAME all_unit_tests COMMAND ut)
