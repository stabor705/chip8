cmake_minimum_required(VERSION 3.21)
project(chip8)

set(CMAKE_CXX_STANDARD 14)

add_executable(chip8 src/main.cpp src/mem.cpp src/mem.h src/chip8.cpp src/chip8.h)
