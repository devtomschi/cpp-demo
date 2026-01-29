# cpp-demo


![](https://github.com/devtomschi/cpp-demo/actions/workflows/ci-build-and-test.yaml/badge.svg)
![](https://gitlab.com/devtomschi/cpp-demo/badges/main/pipeline.svg?ignore_skipped=true&key_text=GitLab+Ubuntu+GCC&key_width=120)


This project demonstrates C++ 17 features and best practices using a set of (small) problems for educational purposes.
It currently contains the following topics:
- [Argument Handling](argument-handling.cpp)

## Requirements
- [CMake](https://cmake.org/) 3.15+ 
- C++ compiler supporting [C++ 17](https://en.cppreference.com/w/cpp/17.html)

### Optional Tools
- [clang-format](https://clang.llvm.org/docs/ClangFormat.html)
- [clang-tidy](https://clang.llvm.org/extra/clang-tidy/)


## Implementation Notes
Topics are typically implemented in a single file including some automatic tests. This file can then be easily be compiled into
a single executable or taken as basis for further experimentation.


## Road Map
Planned improvements and additions are currently tracked as [GitLab issues](https://gitlab.com/devtomschi/cpp-demo/-/issues). 
Ideas regarding interesting problems are collected in the following list:
- Virtual terminal input/output handling