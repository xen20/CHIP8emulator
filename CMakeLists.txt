cmake_minimum_required(VERSION 2.8)

project(CHIP8_Improved)

find_package(SDL2 REQUIRED)
include_directories(CHIP8_Improved ${SDL2_INCLUDE_DIRS})

file(GLOB_RECURSE SRC . *.cpp)
file(GLOB_RECURSE HED . *.h)

add_executable(${PROJECT_NAME} ${SRC} ${HED})
target_link_libraries(CHIP8_Improved ${SDL2_LIBRARIES})
