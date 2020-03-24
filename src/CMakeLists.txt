set(MY_FLAGS "-g -O2 -Wall -Wpedantic -Wpadded")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${MY_FLAGS}")

include_directories("../include")

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# chaos files
file(GLOB chaosFiles
        "${CMAKE_CURRENT_SOURCE_DIR}/chaos/*.cpp")

# transform files
file(GLOB transformFiles
        "${CMAKE_CURRENT_SOURCE_DIR}/transform/*.cpp")

# collage files
file(GLOB collageFiles
        "${CMAKE_CURRENT_SOURCE_DIR}/collage/*.cpp")

# define executables
add_executable(run_fractal main_fractal.cpp ${chaosFiles})
add_executable(run_fern main_fern.cpp ${transformFiles})
add_executable(run_collage main_collage.cpp ${transformFiles} ${collageFiles})

# link custom bmp library
target_link_libraries(run_fractal LINK_PUBLIC bmplib)
target_link_libraries(run_fern LINK_PUBLIC bmplib)
target_link_libraries(run_collage LINK_PUBLIC bmplib)
