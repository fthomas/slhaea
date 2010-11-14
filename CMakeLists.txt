cmake_minimum_required(VERSION 2.6)

project(SLHAea CXX)

set(SLHAea_VERSION_MAJOR "0")
set(SLHAea_VERSION_MINOR "0")
set(SLHAea_VERSION "${SLHAea_VERSION_MAJOR}.${SLHAea_VERSION_MINOR}")

# Enable more warnings if g++, clang or icpc is used.
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER MATCHES clang)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
elseif(CMAKE_CXX_COMPILER MATCHES icpc)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -strict-ansi -Wall -Wcheck")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -wd383 -wd444 -wd981")
endif()

find_package(Boost REQUIRED COMPONENTS unit_test_framework)
find_package(Doxygen)
find_package(LATEX)

enable_testing()
set(CMAKE_VERBOSE_MAKEFILE TRUE)

set(CMAKE_COPY ${CMAKE_COMMAND} -E copy)
set(SLHAEA_H ${CMAKE_SOURCE_DIR}/slhaea.h)

add_subdirectory(doc)
add_subdirectory(test)
