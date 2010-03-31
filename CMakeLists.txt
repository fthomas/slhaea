cmake_minimum_required(VERSION 2.6)

project(SLHAea CXX)

# Enable more warnings if g++ or icpc is used.
if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
elseif(CMAKE_CXX_COMPILER MATCHES icpc)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -strict-ansi -Wall -Wcheck")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -wd383 -wd444 -wd981")
endif()

find_package(Boost REQUIRED COMPONENTS unit_test_framework)

set(CMAKE_VERBOSE_MAKEFILE TRUE)

add_subdirectory(test)
