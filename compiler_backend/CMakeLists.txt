cmake_minimum_required(VERSION 3.20)   # не 4.1, таких версий нет
project(Compiler)

set(CMAKE_CXX_STANDARD 17)             # C++26 слишком новый, используйте 17 или 20
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Compiler
        ast_to_asm.cpp
        AsmGenerator.cpp
        AstParser.cpp
        NamesTable.cpp
)