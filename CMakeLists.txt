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

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
  include_directories("${gtest_SOURCE_DIR}/include")
endif()

add_executable(${PROJECT_NAME} ${SRC} ${HED})
target_link_libraries(CHIP8_Improved ${SDL2_LIBRARY} gtest gtest_main)
