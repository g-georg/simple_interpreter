cmake_minimum_required(VERSION 3.20)

set(FRONTEND_SOURCES
    ../ast_frontend/Parser.cpp
    ../ast_frontend/Tokenizer.cpp
    ../ast_frontend/AstPrinter.cpp
    ../ast_frontend/ast_frontend.cpp
)


set(BACKEND_SOURCES
    ../compiler_backend/AsmGenerator.cpp
    ../compiler_backend/AstParser.cpp
    ../compiler_backend/NamesTable.cpp
    ../compiler_backend/backend.cpp
)

add_executable(compiler
    main.cpp
    ${FRONTEND_SOURCES}
    ${BACKEND_SOURCES}
)

target_include_directories(compiler PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/ast_frontend
    ${CMAKE_SOURCE_DIR}/compiler_backend
)
