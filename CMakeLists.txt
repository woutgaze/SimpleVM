cmake_minimum_required(VERSION 3.12)
project(SimpleVM C)

set(CMAKE_C_STANDARD 11)

add_executable(SimpleVM main.c ObjectMemory.c ObjectMemory.h ASTNodes.c ASTNodes.h Interpreter.c Interpreter.h DevTools.c DevTools.h reader/NodeReader.c reader/NodeReader.h reader/NodeReader_V1.c reader/NodeReader_V1.h reader/NodeReader_V1_Tools.c reader/NodeReader_V1_Tools.h)