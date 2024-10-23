cmake_minimum_required(VERSION 3.16)
project(yadro_tape_test)

set(CMAKE_CXX_STANDARD 17)

add_executable(yadro_tape_test main.cpp interface.h TapeSorter.cpp TapeSorter.h TapeEmulator.cpp TapeEmulator.h)
add_executable(generator generator.cpp)
