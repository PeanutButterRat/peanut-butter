cmake_minimum_required(VERSION 3.27)
project(cpbpl)

set(CMAKE_CXX_STANDARD 17)

add_executable(cpbpl main.cpp scanner)

add_library(scanner scanner.cpp scanner.h)
