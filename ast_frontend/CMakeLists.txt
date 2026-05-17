cmake_minimum_required(VERSION 3.20)
project(ast_frontend CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(src_to_ast
    src_to_ast.cpp
    Parser.cpp
    Tokenizer.cpp
    AstPrinter.cpp
)

target_include_directories(src_to_ast PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_options(src_to_ast PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Wconversion
)
