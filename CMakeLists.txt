cmake_minimum_required(VERSION 3.16)
project(yadro_tape_test)

set(CMAKE_CXX_STANDARD 17)

add_executable(yadro_tape_test main.cpp interface.h TapeSorter.cpp TapeSorter.h TapeEmulator.cpp TapeEmulator.h)
add_executable(generator generator.cpp)
add_executable(test test.cpp interface.h TapeSorter.cpp TapeSorter.h TapeEmulator.cpp TapeEmulator.h)

add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/temp
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/temp
    COMMENT "Creating temp directory"

)

add_custom_target(create_temp_dir ALL
    DEPENDS ${CMAKE_BINARY_DIR}/temp
)

add_dependencies(yadro_tape_test create_temp_dir)
