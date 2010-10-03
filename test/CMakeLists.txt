add_subdirectory(code-analysis)

include_directories(.. ${Boost_INCLUDE_DIRS})

file(GLOB UT_SOURCES ut-*.cpp ut-*.h)
add_executable(ut ${UT_SOURCES} ../slhaea.h)
target_link_libraries(ut ${Boost_LIBRARIES})

add_executable(input input.cpp ../slhaea.h)
target_link_libraries(input ${Boost_LIBRARIES})

configure_file(input.txt . COPYONLY)
