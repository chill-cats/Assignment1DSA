cmake_minimum_required(VERSION 3.20)
project(assignmentDsaAlternative)

set(CMAKE_CXX_STANDARD 14)

if (CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang"
        OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang"
        OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")

        add_compile_options(-Wall -Wextra -pedantic -Wshadow -Wdouble-promotion -Wundef -fno-common)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang"
            OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        add_compile_options(-fsanitize=address -fno-omit-frame-pointer)
        add_link_options(-fsanitize=address)
    endif()
    if (CMAKE_BUILD_TYPE STREQUAL "Release")
        add_compile_options(-Werror)
    endif()
endif()

if (SAN STREQUAL "ASan")
    if (CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang"
            OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        add_compile_options(-fsanitize=address -fno-omit-frame-pointer)
        add_link_options(-fsanitize=address)
    endif()
endif()

if (SAN STREQUAL "UBSan")
    if (CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang"
            OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        add_compile_options(-fsanitize=undefined -fno-omit-frame-pointer)
        add_link_options(-fsanitize=undefined)
    endif()
endif()
find_package(GTest)

if (GTest_FOUND)
    enable_testing()
endif()

add_library(symbolTable SymbolTable.h SymbolTable.cpp)

add_executable(assignmentDsaAlternative main.cpp main.h)
target_link_libraries(assignmentDsaAlternative symbolTable)

if (GTest_FOUND AND (CMAKE_BUILD_TYPE STREQUAL "Debug"))
    add_executable(testInsert testing/test_insert.cpp)
    target_link_libraries(testInsert symbolTable GTest::gtest GTest::gtest_main)
    gtest_discover_tests(testInsert)

    add_executable(testInputOutput testing/test_input_output.cpp)
    target_link_libraries(testInputOutput symbolTable GTest::gtest GTest::gtest_main)
    gtest_discover_tests(testInputOutput)

endif()