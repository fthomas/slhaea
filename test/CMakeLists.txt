add_subdirectory(code-analysis)
add_subdirectory(unit-tests)

include_directories(.. ${Boost_INCLUDE_DIRS})

add_executable(input input.cpp ../slhaea.h)
target_link_libraries(input ${Boost_LIBRARIES})

configure_file(input.txt . COPYONLY)
