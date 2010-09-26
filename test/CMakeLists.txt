include_directories(.. ${Boost_INCLUDE_DIRS})

add_executable(ut ut-main.cpp ut-line.h ut-block.h ut-coll.h ut-key.h
               ../slhaea.h)
target_link_libraries(ut ${Boost_LIBRARIES})

add_executable(input input.cpp ../slhaea.h)
target_link_libraries(input ${Boost_LIBRARIES})

configure_file(input.txt . COPYONLY)
