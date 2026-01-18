cmake_minimum_required(VERSION 3.10)

project(cpp-demo C CXX)
include(CTest)

set(CMAKE_CXX_STANDARD 17)

add_executable(argument-handling argument-handling.cpp)
add_test(NAME argument-handling-basic COMMAND $<TARGET_FILE:argument-handling> 1 2)
    set_property(TEST argument-handling-basic PROPERTY PASS_REGULAR_EXPRESSION "argument count: 3")
add_test(NAME argument-handling-options COMMAND $<TARGET_FILE:argument-handling> -a 2)
    set_property(TEST argument-handling-options PROPERTY PASS_REGULAR_EXPRESSION "option: -a")
add_test(NAME argument-handling-only-positional COMMAND $<TARGET_FILE:argument-handling> -- -a 2)
    set_property(TEST argument-handling-only-positional PROPERTY FAIL_REGULAR_EXPRESSION "option: -a")
