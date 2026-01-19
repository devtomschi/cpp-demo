/*!
 * \file
 * C++ 17 argument handling demo program.
 *
 * Argument handling is a very common problem addressed by different libraries like
 * [Boost program_options](https://www.boost.org/doc/libs/latest/doc/html/program_options.html).
 * The following program implements a simplified argument handling featuring:
 *  - a set of boolean options
 *  - positional arguments
 */

#include <cstdio>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <set>
#include <string_view>
#include <tuple>
#include <utility>
#include <vector>

namespace {
std::tuple<std::vector<std::string_view>, std::set<std::string_view>>
parseArguments(const std::vector<std::string_view> &arguments)
{
    std::set<std::string_view> options;
    std::vector<std::string_view> positional_arguments;

    bool now_only_positional = false;
    for (const auto &arg : std::as_const(arguments)) {
        if (arg == "--") {
            now_only_positional = true;
            continue;
        }
        if (!now_only_positional && arg[0] == '-')
            options.insert(arg);
        else
            positional_arguments.push_back(arg);
    }
    return {positional_arguments, options};
}

int runTests()
{
    int return_value = EXIT_SUCCESS;
    // NOLINTNEXTLINE(readability-identifier-naming)
    auto checkThat = [&return_value](auto expr, int line = __builtin_LINE(), const char *file = __builtin_FILE()) {
        if (!expr) {
            return_value = EXIT_FAILURE;
            std::fprintf(stderr, "%s:%i: check failed!\n", file, line);
        }
    };
    {
        auto [positionals, options] = parseArguments({"myexe", "1", "2"});
        checkThat(options.empty());
        checkThat(positionals.size() == 3);
    }
    {
        auto [positionals, options] = parseArguments({"myexe", "-a", "2"});
        checkThat(options.find("-a") != options.end());
        checkThat(positionals.size() == 2);
    }
    {
        auto [positionals, options] = parseArguments({"myexe", "--", "-a", "2"});
        checkThat(options.empty());
        checkThat(positionals.size() == 3);
        checkThat(positionals[1] == "-a");
    }
    return return_value;
}

} // namespace

int main(int argc, char *argv[])
try {
    const std::vector<std::string_view> arguments{argv, argv + argc};

    if (arguments.size() >= 2 && arguments[1] == "-test")
        return runTests();

    auto [positional_arguments, options] = parseArguments(arguments);

    std::cout << "positional argument count: " << positional_arguments.size() << '\n';
    for (const auto &arg : std::as_const(positional_arguments))
        std::cout << "positional argument: " << arg << '\n';

    if (options.find("-a") != options.end())
        std::cout << "option: -a " << '\n';

    return EXIT_SUCCESS;
}
catch (std::exception &e) {
    std::fprintf(stderr, "caught exception: '%s', exiting...\n", e.what());
    return EXIT_FAILURE;
}
