cmake_minimum_required(VERSION 3.17)
project(untitled26)

set(CMAKE_CXX_STANDARD 14)

add_executable(untitled26 main.cpp main.h)
add_library(symbolTable SymbolTable.h SymbolTable.cpp)

target_link_libraries(untitled26 symbolTable)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU"
        AND CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(untitled26 -fsanitize=address -fno-omit-frame-pointer)
    target_compile_options(symbolTable -fsanitize=address -fno-omit-frame-pointer)
    target_link_options(untitled26 -fsanitize=address -fno-omit-frame-pointer)
    target_link_options(symbolTable -fsanitize=address -fno-omit-frame-pointer)
endif()
