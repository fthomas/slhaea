configure_file(slha1.txt . COPYONLY)
configure_file(slha2.txt . COPYONLY)

include_directories(${CMAKE_SOURCE_DIR} ${Boost_INCLUDE_DIRS})

file(GLOB EX_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)
foreach(EX_SRC ${EX_SOURCES})
    string(REPLACE ".cpp" "" EX_BINARY ${EX_SRC})
    list(APPEND EX_BINARIES ${EX_BINARY})
    add_executable(${EX_BINARY} EXCLUDE_FROM_ALL ${EX_SRC} ${SLHAEA_H})
endforeach()

list(SORT EX_BINARIES)
add_custom_target(examples DEPENDS ${EX_BINARIES})
foreach(EX_BINARY ${EX_BINARIES})
    add_custom_command(TARGET examples POST_BUILD COMMAND ${EX_BINARY})
endforeach()
