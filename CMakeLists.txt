cmake_minimum_required(VERSION 3.10)

project(cpp-demo C CXX)
include(CTest)

set(CMAKE_CXX_STANDARD 17)

add_executable(argument-handling argument-handling.cpp)
add_test(NAME argument-handling-test COMMAND $<TARGET_FILE:argument-handling> 1 2 3)
    set_property(TEST argument-handling-test PROPERTY PASS_REGULAR_EXPRESSION "argc: 4")
