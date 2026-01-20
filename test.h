/*!
 * \file
 * Minimalistic test framework.
 */

#pragma once

#include <cstdio>
#include <cstdlib>
#include <string_view>

struct Test {
    Test() = default;
    ~Test() = default;

    void operator()(bool expr, int line = __builtin_LINE(), const char *file = __builtin_FILE())
    {
        if (!expr) {
            result = EXIT_FAILURE;
            std::fprintf(stderr, "%s:%i: check failed!\n", file, line);
        }
    }
    // NOLINTNEXTLINE(modernize-avoid-c-arrays)
    template <typename F> void runIfRequested(int argc, char *argv[], F run_tests)
    {
        using namespace std::string_view_literals;
        if (argc >= 2 && argv[1] == "-test"sv) {
            run_tests();
            std::exit(result);
        }
    }

    int result = EXIT_SUCCESS;
} test;
