cmake_minimum_required(VERSION 3.21)
project(chip8)

set(CMAKE_CXX_STANDARD 17)

file(GLOB chip8_SOURCE_FILES src/*.cpp)
file(GLOB chip8_HEADER_FILES src/*.h)

include_directories(src)
add_executable(chip8 ${chip8_SOURCE_FILES})

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_subdirectory(extern/spdlog)
target_link_libraries(chip8 ${SDL2_LIBRARIES} spdlog::spdlog)

add_subdirectory(extern/googletest)
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
add_executable(unitTests tests/test.cpp ${chip8_SOURCE_FILES})
target_link_libraries(unitTests gtest gtest_main)
add_test(unitTests unitTests)