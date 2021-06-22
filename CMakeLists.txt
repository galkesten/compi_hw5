cmake_minimum_required(VERSION 3.13)
project(compi_hw5)

set(CMAKE_CXX_STANDARD 14)

add_executable(compi_hw5 Instructions.cpp Instructions.h bp.hpp bp.cpp code_gen.cpp code_gen.hpp semanticAnalyzer.h semanticAnalyzer.cpp)