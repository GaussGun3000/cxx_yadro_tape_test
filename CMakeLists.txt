cmake_minimum_required(VERSION 3.20)
project(yadro_tape_test)

set(CMAKE_CXX_STANDARD 14)

add_executable(yadro_tape_test main.cpp interface.h TapeSorter.cpp TapeSorter.h Tape.cpp Tape.h)
