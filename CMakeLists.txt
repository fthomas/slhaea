cmake_minimum_required(VERSION 2.6)

project(SLHAea CXX)

# Enable more warnings if GCC is used.
if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
endif()

find_package(Boost REQUIRED COMPONENTS unit_test_framework)

set(CMAKE_VERBOSE_MAKEFILE TRUE)

add_subdirectory(test)
