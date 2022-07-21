cmake_minimum_required(VERSION 3.21)
project(chip8)

set(CMAKE_CXX_STANDARD 17)

include_directories(src)
add_executable(chip8 src/main.cpp src/mem.cpp src/mem.h src/chip8.cpp src/chip8.h src/emulation.cpp src/emulation.h)

add_subdirectory(extern/googletest)
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
add_executable(unitTests tests/test.cpp src/chip8.h src/chip8.cpp src/mem.cpp src/mem.h)
target_link_libraries(unitTests gtest gtest_main)
add_test(unitTests unitTests)