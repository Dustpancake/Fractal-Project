# include exterior headers
include_directories("../include")

# set bmpfiles as variable
file(GLOB bmpfiles
        "${CMAKE_CURRENT_SOURCE_DIR}/bmplib/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/bmplib/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

set(MY_FLAGS "-g -O2 -Wall -Wpedantic")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${MY_FLAGS}")

# construct into a static library
add_library(bmplib SHARED ${bmpfiles})
target_include_directories (bmplib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})