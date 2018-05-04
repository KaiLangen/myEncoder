enable_testing()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
find_package(Threads REQUIRED)
find_package(GTest REQUIRED)

include_DIRECTORIES(${GTEST_INCLUDE_DIRS})

if(NOT MSVC)
  set(PThreadLib -pthread)
endif()

add_executable( dctTest dct_test.cc )
target_link_libraries( dctTest utils ${PThreadLib} ${GTEST_BOTH_LIBRARIES} )
add_test( dctTest dctTest )

add_executable( vlcTest vlc_test.cc )
target_link_libraries( vlcTest utils ${PThreadLib} ${GTEST_BOTH_LIBRARIES} )
add_test( vlcTest vlcTest )

add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} DEPENDS vlcTest dctTest)
