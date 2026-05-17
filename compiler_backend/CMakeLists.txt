cmake_minimum_required(VERSION 3.20)  
project(Compiler)

set(CMAKE_CXX_STANDARD 17)             
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Compiler
        ast_to_asm.cpp
        AsmGenerator.cpp
        AstParser.cpp
        NamesTable.cpp
)