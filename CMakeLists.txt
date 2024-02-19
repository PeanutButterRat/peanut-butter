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

target_link_libraries(cpbpl PRIVATE scanner)
