/*!
 * \file
 * Minimal, project-specific test support.
 */

#pragma once

#include <cstdio>
#include <cstdlib>
#include <string_view>

/*!
 * Represents state of tests.
 */
class Tests {
    Tests() = delete;
    ~Tests() = delete;

  public:
    /*!
     * Run given test function if requested on the command line.
     */
    // NOLINTNEXTLINE(modernize-avoid-c-arrays)
    template <typename F> static void runIfRequested(int argc, char *argv[], F run_tests)
    {
        using namespace std::string_view_literals;
        if (argc >= 2 && argv[1] == "-test"sv) {
            run_tests();
            std::exit(result_);
        }
    }

    friend bool checkThat(bool expr, int line, const char *file);

  private:
    static inline int result_ = EXIT_SUCCESS;
};

/*!
 * Check that given expression is true.
 * If test expression is not true, line and file name are logged to stderr and the test run fails.
 */
inline bool checkThat(bool expr, int line = __builtin_LINE(), const char *file = __builtin_FILE())
{
    if (!expr) {
        Tests::result_ = EXIT_FAILURE;
        std::fprintf(stderr, "%s:%i: check failed!\n", file, line);
        return false;
    }
    else
        return true;
}
