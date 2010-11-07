include_directories(${CMAKE_SOURCE_DIR})
add_executable(linkage bar.cpp foo.cpp main.cpp ${SLHAEA_H})
