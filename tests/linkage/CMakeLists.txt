include_directories(${CMAKE_SOURCE_DIR} ${Boost_INCLUDE_DIRS})
add_executable(linkage bar.cpp foo.cpp foobar.h main.cpp ${SLHAEA_H})
