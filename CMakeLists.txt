cmake_minimum_required(VERSION 3.20)
project(assignmentDsaAlternative)

set(CMAKE_CXX_STANDARD 17)

if (CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang"
        OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang"
        OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")

        add_compile_options(-Wall -Wextra -pedantic -Wdouble-promotion -Wundef -fno-common)
    if (CMAKE_BUILD_TYPE STREQUAL "Release")
        add_compile_options(-Werror)
    endif()
endif()

if (SAN STREQUAL "ASan")
    if (CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang"
            OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang"
            OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
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


add_library(symbolTable SymbolTable.h SymbolTable.cpp)

add_executable(assignmentDsaAlternative main.cpp main.h)
target_link_libraries(assignmentDsaAlternative symbolTable)

add_library(symbolTableHQ testing/HQ/SymbolTable.cpp testing/HQ/SymbolTable.h)
add_executable(mainHQ testing/HQ/mainQ.cpp testing/HQ/main.h)
target_link_libraries(mainHQ symbolTableHQ)

#if (GTest_FOUND AND (CMAKE_BUILD_TYPE STREQUAL "Debug"))
#    enable_testing()
#
#    add_executable(testInsert testing/test_insert.cpp)
#    target_link_libraries(testInsert symbolTable GTest::gtest GTest::gtest_main)
#    gtest_discover_tests(testInsert)
#
#    add_executable(testInputOutput testing/test_input_output.cpp)
#    target_link_libraries(testInputOutput symbolTable GTest::gtest GTest::gtest_main)
#    gtest_discover_tests(testInputOutput)
#
#endif()