cmake_minimum_required(VERSION 3.12)
project(SimpleVM C)

set(CMAKE_C_STANDARD 11)

add_executable(SimpleVM main.c ObjectMemory.c ObjectMemory.h ASTNodes.c ASTNodes.h Interpreter.c Interpreter.h)