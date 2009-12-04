find_package(Boost REQUIRED)
include_directories(..)
add_executable(testall testall.cpp)
target_link_libraries(testall cppunit)
