cmake_minimum_required(VERSION 3.21)
project(chip8)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE chip8_SOURCE_FILES src/*.cpp)

add_executable(chip8 ${chip8_SOURCE_FILES})
include_directories(src)
include_directories(extern/imgui)
include_directories(extern/imgui/backends)
include_directories(extern/glad/include)

add_library(glad extern/glad/src/glad.cpp)

file(GLOB imgui_SOURCE_FILES extern/imgui/imgui*.cpp)
set(imgui_BACKEND extern/imgui/backends/imgui_impl_opengl3.cpp
                  extern/imgui/backends/imgui_impl_glfw.cpp)
add_library(imgui ${imgui_SOURCE_FILES} ${imgui_BACKEND})

target_link_libraries(chip8 glad glfw imgui)

#add_subdirectory(extern/googletest)
#enable_testing()
#include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
#add_executable(unitTests tests/test.cpp ${chip8_SOURCE_FILES})
#target_link_libraries(unitTests gtest gtest_main)
#add_test(unitTests unitTests)
