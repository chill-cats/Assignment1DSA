cmake_minimum_required(VERSION 3.18)
project(assignment_1_DSA CXX)

add_library(SymbolTable SymbolTable.cpp SymbolTable.h)

add_executable(main main.h mainQ.cpp)

target_link_libraries(main SymbolTable)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(main PRIVATE -fsanitize=address -fno-omit-frame-pointer)
    target_compile_options(SymbolTable PRIVATE -fsanitize=address -fno-omit-frame-pointer)

    target_link_options(main PRIVATE -fsanitize=address -fno-omit-frame-pointer)
    target_link_options(SymbolTable PRIVATE -fsanitize=address -fno-omit-frame-pointer)
endif()
