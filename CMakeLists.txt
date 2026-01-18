cmake_minimum_required(VERSION 3.10)

project(cpp-demo C CXX)
include(CTest)

set(CMAKE_CXX_STANDARD 17)

add_executable(argument-handling argument-handling.cpp)
add_test(NAME argument-handling-basic COMMAND $<TARGET_FILE:argument-handling> 1 2)
    set_property(TEST argument-handling-basic PROPERTY PASS_REGULAR_EXPRESSION "argument count: 3")
add_test(NAME argument-handling-flags COMMAND $<TARGET_FILE:argument-handling> -a 2)
    set_property(TEST argument-handling-flags PROPERTY PASS_REGULAR_EXPRESSION "argument flag: -a")
