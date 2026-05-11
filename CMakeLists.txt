cmake_minimum_required(VERSION 3.20)
project(simple_interpreter CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(asm
  asm/source/main.cpp
  asm/source/assembler.cpp
)
target_include_directories(asm PRIVATE
  ${CMAKE_SOURCE_DIR}/asm/include
)

target_compile_options(asm PRIVATE -Wall -Wextra -Wpedantic)