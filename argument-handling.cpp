/*!
 * \file
 * C++ 17 argument handling demo program.
 *
 * Argument handling is a very common problem addressed by different libraries like
 * [Boost program_options](https://www.boost.org/doc/libs/latest/doc/html/program_options.html) and many others.
 * This program implements simple argument parsing with a limited feature set.
 */

#include "test.h"

#include <cstdio>
#include <cstdlib>
#include <exception>
#include <initializer_list>
#include <iostream>
#include <set>
#include <string_view>
#include <tuple>
#include <utility>
#include <vector>

namespace {

/*!
 * Argument parsing featuring:
 *  - boolean options (starting with at least a single dash)
 *  - positional arguments
 *  - double dash denotes that all subsequents arguments are positional
 */
std::tuple<std::vector<std::string_view>, std::set<std::string_view>> parseArguments(int argc, char **argv)
{
    const std::vector<std::string_view> arguments{argv, argv + argc};
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

void testParseArguments()
{
    auto parse_arguments = [](std::initializer_list<const char *> args) {
        return parseArguments(static_cast<int>(args.size()), const_cast<char **>(std::data(args)));
    };
    {
        auto [positionals, options] = parse_arguments({"myexe", "1", "2"});
        checkThat(options.empty());
        checkThat(positionals.size() == 3);
    }
    {
        auto [positionals, options] = parse_arguments({"myexe", "-a", "2"});
        checkThat(options.find("-a") != options.end());
        checkThat(positionals.size() == 2);
    }
    {
        auto [positionals, options] = parse_arguments({"myexe", "--", "-a", "2"});
        checkThat(options.empty());
        checkThat(positionals.size() == 3);
        checkThat(positionals[1] == "-a");
    }
}

} // namespace

int main(int argc, char *argv[])
try {
    Tests::runIfRequested(argc, argv, testParseArguments);

    auto [positional_arguments, options] = parseArguments(argc, argv);

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
