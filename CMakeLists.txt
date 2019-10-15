set(CMAKE_VERBOSE_MAKEFILE ON)
cmake_minimum_required(VERSION 3.6)
set(CMAKE_CXX_STANDARD 11)

project(CHIP8_Improved)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CHIP8_Improved_SOURCE_DIR}/cmake")

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    # Check 64-bit or 32-bit architecture, select collect libs
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(SDL2_PATH "C:\\SDL2\\x86_64-w64-mingw32")
    elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
        set(SDL2_PATH "C:\\SDL2\\i686-w64-mingw32")
    endif()
endif()

find_package(SDL2 REQUIRED)
include_directories(CHIP8_Improved ${SDL2_INCLUDE_DIR})

file(GLOB_RECURSE SRC . *.cpp)
file(GLOB_RECURSE HED . *.h)

add_executable(${PROJECT_NAME} ${SRC} ${HED})
target_link_libraries(CHIP8_Improved ${SDL2_LIBRARIES})
