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
#include <map>
#include <string_view>
#include <utility>
#include <vector>

namespace {

/*!
 * Argument parsing featuring:
 *  - boolean options (starting with at least a single dash)
 *  - positional arguments
 *  - double dash denotes that all subsequents arguments are positional
 */
std::vector<std::string_view> parseArguments(int argc, char **argv, std::map<std::string_view, bool> &program_options)
{
    const std::vector<std::string_view> arguments{argv, argv + argc};
    std::vector<std::string_view> positional_arguments;

    bool now_only_positional = false;
    for (const auto &arg : std::as_const(arguments)) {
        if (arg == "--") {
            now_only_positional = true;
            continue;
        }
        if (!now_only_positional && arg[0] == '-') {
            if (const auto it = program_options.find(arg); it != program_options.end())
                it->second = true;
            else
                ; // silently discard unknown options for now
        }
        else {
            positional_arguments.push_back(arg);            
        }
    }
    return positional_arguments;
}

void testParseArguments()
{
    auto parse_arguments = [](std::initializer_list<const char *> args, std::map<std::string_view, bool> &program_options) {
        return parseArguments(static_cast<int>(args.size()), const_cast<char **>(std::data(args)), program_options);
    };
    {
        std::map<std::string_view, bool> program_options{{"-a", false}, {"-b", false}};
        auto positionals = parse_arguments({"myexe", "1", "2"}, program_options);
        checkThat(!program_options["-a"]);
        checkThat(!program_options["-b"]);
        checkThat(positionals.size() == 3);
    }
    {
        std::map<std::string_view, bool> program_options{{"-a", false}, {"-b", false}};
        auto positionals = parse_arguments({"myexe", "-a", "2"}, program_options);
        checkThat(program_options["-a"]);
        checkThat(!program_options["-b"]);
        checkThat(positionals.size() == 2);
    }
    {
        std::map<std::string_view, bool> program_options{{"-a", false}, {"-b", false}};
        auto positionals = parse_arguments({"myexe", "-b", "--", "-a", "2"}, program_options);
        checkThat(!program_options["-a"]);
        checkThat(program_options["-b"]);
        checkThat(positionals.size() == 3);
        checkThat(positionals[1] == "-a");
    }
}

} // namespace

int main(int argc, char *argv[])
try {
    Tests::runIfRequested(argc, argv, testParseArguments);

    std::map<std::string_view, bool> program_options{{"-a", false}, {"-b", false}};
    auto positional_arguments = parseArguments(argc, argv, program_options);

    std::cout << "positional argument count: " << positional_arguments.size() << '\n';
    for (const auto &arg : std::as_const(positional_arguments))
        std::cout << "positional argument: " << arg << '\n';

    std::cout << "option a: " << program_options["-a"] << '\n';
    std::cout << "option b: " << program_options["-b"] << '\n';

    return EXIT_SUCCESS;
}
catch (std::exception &e) {
    std::fprintf(stderr, "caught exception: '%s', exiting...\n", e.what());
    return EXIT_FAILURE;
}
