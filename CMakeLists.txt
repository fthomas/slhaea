cmake_minimum_required(VERSION 2.6)
set(CMAKE_VERBOSE_MAKEFILE TRUE)
project(SLHApp CXX)

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
endif()

add_subdirectory(test)
