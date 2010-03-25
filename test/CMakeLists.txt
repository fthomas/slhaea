include_directories(.. ${Boost_INCLUDE_DIRS})

add_executable(testall testall.cpp slhaline.h slhablock.h slha.h ../slhaea.h)
target_link_libraries(testall ${Boost_LIBRARIES})

add_executable(input input.cpp ../slhaea.h)
target_link_libraries(input ${Boost_LIBRARIES})

configure_file(input.txt . COPYONLY)
