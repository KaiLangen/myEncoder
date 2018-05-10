cmake_minimum_required(VERSION 2.8)

project( encoder )
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
configure_file(config.h.in ${CMAKE_BINARY_DIR}/generated/config.h)
include_directories(${CMAKE_BINARY_DIR}/generated/)

add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(sandbox)

