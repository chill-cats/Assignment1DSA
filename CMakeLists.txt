cmake_minimum_required(VERSION 3.17)
project(untitled26)

set(CMAKE_CXX_STANDARD 14)

add_library(symbolTable SymbolTable.cpp SymbolTable.h)
add_executable(main main.cpp)

target_link_libraries(main PRIVATE symbolTable)
