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
#include <utility>
#include <vector>

int main(int argc, char *argv[])
try {
    std::vector<std::string_view> arguments{argv, argv + argc};
    std::cout << "argument count: " << arguments.size() << '\n';

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
