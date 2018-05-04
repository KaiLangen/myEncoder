set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")

add_library(utils dct.c vlc.c)
target_link_libraries(utils m)

add_executable(encoder encoder.c)
target_link_libraries(encoder utils)
