cmake_minimum_required(VERSION 3.20)

project(spu LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(SPU_BUILD_TESTS "Build SPU tests" ON)

add_library(spu_lib
    src/core/spu.cpp
    src/commands/command_base.cpp
    src/commands/command_dispatcher.cpp
    src/commands/jump_commands.cpp
    src/commands/math_commands.cpp
    src/commands/stack_commands.cpp
)

target_include_directories(spu_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_options(spu_lib PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)

add_executable(spu_runner
    src/main.cpp
)

target_link_libraries(spu_runner PRIVATE
    spu_lib
)

target_compile_options(spu_runner PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)

if(SPU_BUILD_TESTS)
    include(FetchContent)

    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
	DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )

    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    FetchContent_MakeAvailable(googletest)

    enable_testing()

    add_executable(spu_tests
        tests/stack_tests.cpp
        tests/math_commands_tests.cpp
        tests/stack_commands_tests.cpp
        tests/spu_tests.cpp
    )

    target_link_libraries(spu_tests PRIVATE
        spu_lib
        GTest::gtest_main
    )

    target_compile_options(spu_tests PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )

    include(GoogleTest)

    gtest_discover_tests(spu_tests)
endif()