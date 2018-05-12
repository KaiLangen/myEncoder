set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")

add_library(utils dct.c vlc.c io.c mb.c tss.c mc.c)
target_link_libraries(utils m)

