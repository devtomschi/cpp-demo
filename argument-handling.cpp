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

/*!
 * Responsible for argument handling (and storage).
 */
class Arguments {
  public:
    Arguments() = default;
    ~Arguments() = default;

    Arguments &addOption(std::string_view option_name, bool default_value)
    {
        options_[option_name] = default_value;
        return *this;
    }

    /*!
     * Argument parsing featuring:
     *  - boolean options (starting with at least a single dash)
     *  - positional arguments
     *  - double dash denotes that all subsequents arguments are positional
     */
    void parse(int argc, char **argv)
    {
        const std::vector<std::string_view> arguments{argv, argv + argc};

        bool now_only_positional = false;
        for (const auto &arg : std::as_const(arguments)) {
            if (arg == "--") {
                now_only_positional = true;
                continue;
            }
            if (!now_only_positional && arg[0] == '-') {
                if (const auto it = options_.find(arg); it != options_.end())
                    it->second = true;
                else
                    ; // silently discard unknown options for now
            }
            else {
                positionals_.push_back(arg);
            }
        }
    }

    bool option(std::string_view option_name) { return options_[option_name]; }
    const std::vector<std::string_view>& positionals() { return positionals_; }

  private:
    std::map<std::string_view, bool> options_;
    std::vector<std::string_view> positionals_;
};

namespace {

void testParseArguments()
{
    auto parse_arguments = [](Arguments &args, std::initializer_list<const char *> cmdline_args) {
        args.parse(static_cast<int>(cmdline_args.size()), const_cast<char **>(std::data(cmdline_args)));
    };
    {
        Arguments args;
        args.addOption("-a", false).addOption("-b", false);
        parse_arguments(args, {"myexe", "1", "2"});
        checkThat(!args.option("-a"));
        checkThat(!args.option("-b"));
        checkThat(args.positionals().size() == 3);
    }
    {
        Arguments args;
        args.addOption("-a", false).addOption("-b", false);
        parse_arguments(args, {"myexe", "-a", "2"});
        checkThat(args.option("-a"));
        checkThat(!args.option("-b"));
        checkThat(args.positionals().size() == 2);
    }
    {
        Arguments args;
        args.addOption("-a", false).addOption("-b", false);
        parse_arguments(args, {"myexe", "-b", "--", "-a", "2"});
        checkThat(!args.option("-a"));
        checkThat(args.option("-b"));
        checkThat(args.positionals().size() == 3);
        checkThat(args.positionals()[1] == "-a");
    }
}

} // namespace

int main(int argc, char *argv[])
try {
    Tests::runIfRequested(argc, argv, testParseArguments);

    Arguments args;
    args.addOption("-a", false).addOption("-b", false);
    args.parse(argc, argv);

    std::cout << "positional argument count: " << args.positionals().size() << '\n';
    for (const auto &arg : std::as_const(args.positionals()))
        std::cout << "positional argument: " << arg << '\n';

    std::cout << "option a: " << args.option("-a") << '\n';
    std::cout << "option b: " << args.option("-b") << '\n';

    return EXIT_SUCCESS;
}
catch (std::exception &e) {
    std::fprintf(stderr, "caught exception: '%s', exiting...\n", e.what());
    return EXIT_FAILURE;
}
