cmake_minimum_required(VERSION 3.0)

project(SLHAea CXX)
string(TOLOWER ${CMAKE_PROJECT_NAME} PROJECT_NAME_LOWER)

set(SLHAea_VERSION_MAJOR "0")
set(SLHAea_VERSION_MINOR "1")
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
add_subdirectory(tests)

# CPack settings:
set(CPACK_PACKAGE_VENDOR "Frank S. Thomas")
set(CPACK_PACKAGE_VERSION_MAJOR ${SLHAea_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${SLHAea_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION       ${SLHAea_VERSION})
set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_SOURCE_DIR}/LICENSE_1_0.txt)
set(CPACK_RESOURCE_FILE_README  ${CMAKE_SOURCE_DIR}/README.md)
set(CPACK_SOURCE_PACKAGE_FILE_NAME "${PROJECT_NAME_LOWER}-${SLHAea_VERSION}")
set(CPACK_SOURCE_GENERATOR TGZ ZIP)
include(CPack)

set(ARCHIVE_FILE_NAME ${CPACK_SOURCE_PACKAGE_FILE_NAME})
add_custom_target(dist
  COMMAND git archive --format=tar --prefix=${ARCHIVE_FILE_NAME}/ HEAD |
    gzip -9 > ${CMAKE_BINARY_DIR}/${ARCHIVE_FILE_NAME}.tar.gz
  COMMAND git archive --format=zip --prefix=${ARCHIVE_FILE_NAME}/
    -9 -o ${CMAKE_BINARY_DIR}/${ARCHIVE_FILE_NAME}.zip HEAD
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
