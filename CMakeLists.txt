cmake_minimum_required(VERSION 3.15)

project(cpp-demo C CXX)
include(CTest)

if(MSVC)
  add_compile_options(/W4)
else()
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(CMAKE_CXX_STANDARD 17)

add_executable(argument-handling argument-handling.cpp)
add_test(NAME argument-handling-test COMMAND $<TARGET_FILE:argument-handling> -test)

