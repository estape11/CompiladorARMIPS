cmake_minimum_required(VERSION 3.12)
project(CompiladorARMIPS)

set(CMAKE_CXX_STANDARD 14)

add_executable(CompiladorARMIPS main.cpp InstructionHelper.cpp InstructionHelper.h BaseHelper.cpp BaseHelper.h)