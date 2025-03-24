cmake_minimum_required(VERSION 3.10)
project(UPLLexer)

# Bật C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Tìm Google Test
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Thêm các file nguồn
add_executable(lexer_test 
    lexer_test.cpp
    lexer.cpp
)

# Link với Google Test
target_link_libraries(lexer_test ${GTEST_LIBRARIES} pthread)