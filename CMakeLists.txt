cmake_minimum_required(VERSION 3.27)
project(cpbpl)

set(CMAKE_CXX_STANDARD 17)

add_library(
        scanner
        src/scanner.cpp
        include/scanner.h
)

add_executable(
        cpbpl
        src/main.cpp
)

target_link_libraries(
        cpbpl
        scanner
)


# Google Test.
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/5df0241ea4880e5a846775d3efc8b873f7b36c31.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
        scanner_test
        test/scanner_test.cc
)
target_link_libraries(
        scanner_test
        scanner
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(scanner_test)
