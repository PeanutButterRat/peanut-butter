cmake_minimum_required(VERSION 3.27)
project(cpbpl)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(
        cpbpl
        compiler/src/main.cpp
)

add_executable(
        pbpl
        vm/src/main.cpp
)

add_library(
        vm
        vm/src/vm.cpp
        vm/include/vm.h
)

add_library(
        value
        compiler/src/value.cpp
        compiler/include/value.h
)

add_library(
        compiler
        compiler/src/compiler.cpp
        compiler/include/compiler.h
)

add_library(
        bytecode
        compiler/src/bytecode.cpp
        compiler/include/bytecode.h
)

add_library(
        scanner
        compiler/src/scanner.cpp
        compiler/include/scanner.h
)

add_library(
        token
        compiler/src/token.cpp
        compiler/include/token.h
)

target_link_libraries(
        cpbpl
        scanner
        bytecode
        compiler
        token
        value
)

target_link_libraries(
        pbpl
        bytecode
        value
        vm
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
        compiler/test/scanner_test.cc
)

target_link_libraries(
        scanner_test
        scanner
        token
        GTest::gtest_main
)

add_executable(
        compiler_test
        compiler/test/compiler_test.cc
)

target_link_libraries(
        compiler_test
        compiler
        value
        scanner
        token
        bytecode
        GTest::gtest_main
)

add_executable(
        vm_test
        vm/test/vm_test.cc
)

target_link_libraries(
        vm_test
        vm
        bytecode
        value
        GTest::gtest_main
)


include(GoogleTest)
gtest_discover_tests(scanner_test)
gtest_discover_tests(compiler_test)
gtest_discover_tests(vm_test)
