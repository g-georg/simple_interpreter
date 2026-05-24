cmake_minimum_required(VERSION 3.20)
project(simple_interpreter CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ── ASM ──────────────────────────────────────────────────────────────────────
add_executable(asm
  asm/source/main.cpp
  asm/source/assembler.cpp
)

target_include_directories(asm PRIVATE
  ${CMAKE_SOURCE_DIR}/asm/include
  ${CMAKE_SOURCE_DIR}/common
)

target_compile_options(asm PRIVATE -Wall -Wextra -Wpedantic)

# ── SPU ──────────────────────────────────────────────────────────────────────
add_library(spu_lib
  spu/src/core/spu.cpp
  spu/src/commands/command_base.cpp
  spu/src/commands/command_dispatcher.cpp
  spu/src/commands/jump_commands.cpp
  spu/src/commands/math_commands.cpp
  spu/src/commands/stack_commands.cpp
)

target_include_directories(spu_lib PUBLIC
  ${CMAKE_SOURCE_DIR}/spu/include
  ${CMAKE_SOURCE_DIR}/common
)

target_compile_options(spu_lib PRIVATE -Wall -Wextra -Wpedantic)

add_executable(spu_runner
  spu/src/main.cpp
)

target_link_libraries(spu_runner PRIVATE spu_lib)
target_compile_options(spu_runner PRIVATE -Wall -Wextra -Wpedantic)

# ── TESTS ────────────────────────────────────────────────────────────────────
option(SPU_BUILD_TESTS "Build SPU tests" ON)

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
    spu/tests/stack_tests.cpp
    spu/tests/math_commands_tests.cpp
    spu/tests/stack_commands_tests.cpp
    spu/tests/spu_tests.cpp
  )

  target_link_libraries(spu_tests PRIVATE
    spu_lib
    GTest::gtest_main
  )

  target_compile_options(spu_tests PRIVATE -Wall -Wextra -Wpedantic)

  include(GoogleTest)
  gtest_discover_tests(spu_tests)
endif()
