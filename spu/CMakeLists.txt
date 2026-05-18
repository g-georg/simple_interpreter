cmake_minimum_required(VERSION 3.16)

project(spu_project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(spu_runner
    src/main.cpp
    src/core/spu.cpp
    src/commands/command_dispatcher.cpp
    src/commands/command_base.cpp
    src/commands/jump_commands.cpp
    src/commands/math_commands.cpp
    src/commands/stack_commands.cpp
)

target_include_directories(spu_runner
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if(MSVC)
    target_compile_options(spu_runner PRIVATE /W4)
else()
    target_compile_options(spu_runner PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_custom_target(run
    COMMAND spu_runner ${CMAKE_CURRENT_SOURCE_DIR}/program.spu
    DEPENDS spu_runner
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)