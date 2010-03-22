include_directories(.. ${Boost_INCLUDE_DIRS})
add_executable(testall testall.cpp)
target_link_libraries(testall ${Boost_LIBRARIES})
