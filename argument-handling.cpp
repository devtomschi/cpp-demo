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
 * Responsible for argument handling and storage.
 *
 * Arguments fall into two categories:
 * - *Options* start with a dash (e.g. -myopt) and can appear in random order
 * - *Positionals* are recorded in the order they appear on the command line
 *
 * **Design Remarks**
 * - Option values are stored inside this class. This is a pragmatic approach for solving the problem in the scope of this demo
 *   program, but is not a general, product-level solution, which might want to store the data somewhere else.
 */
class Arguments {
  public:
    using PositionalContainer = std::vector<std::string_view>;

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
                auto assignment_index = arg.find('=');
                std::string_view option_name;
                bool option_value = true;
                if (assignment_index != std::string_view::npos) {
                    option_name = arg.substr(0, assignment_index);
                    const std::string_view value_string = arg.substr(assignment_index + 1);
                    if (value_string == "true" || value_string == "1")
                        option_value = true;
                    else if (value_string == "false" || value_string == "0")
                        option_value = false;
                }
                else {
                    option_name = arg;
                }

                if (const auto it = options_.find(option_name); it != options_.end())
                    it->second = option_value;
                else
                    ; // FIXME: silently discard unknown options for now
            }
            else {
                positionals_.push_back(arg);
            }
        }
    }

    /*! Access options by name */
    bool &operator[](std::string_view option_name) { return options_[option_name]; }
    /*! Access positionals by index */
    std::string_view operator[](int index) const { return positionals_[index]; }
    [[nodiscard]] PositionalContainer::const_iterator begin() const { return positionals_.cbegin(); }
    [[nodiscard]] PositionalContainer::const_iterator end() const { return positionals_.cend(); }
    [[nodiscard]] PositionalContainer::size_type size() const { return positionals_.size(); }

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
        checkThat(!args["-a"]);
        checkThat(!args["-b"]);
        checkThat(args.size() == 3);
    }
    {
        Arguments args;
        args.addOption("-a", false).addOption("-b", false);
        parse_arguments(args, {"myexe", "-a", "2"});
        checkThat(args["-a"]);
        checkThat(!args["-b"]);
        checkThat(args.size() == 2);
    }
    {
        Arguments args;
        args.addOption("-a", false).addOption("-b", false);
        parse_arguments(args, {"myexe", "-b", "--", "-a", "2"});
        checkThat(!args["-a"]);
        checkThat(args["-b"]);
        checkThat(args.size() == 3);
        checkThat(args[1] == "-a");
    }
    {
        Arguments args;
        args.addOption("-a", true).addOption("-b", true).addOption("-c", false).addOption("-d", false);
        parse_arguments(args, {"myexe", "-a=0", "-b=false", "-c=1", "-d=true"});
        checkThat(!args["-a"]);
        checkThat(!args["-b"]);
        checkThat(args["-c"]);
        checkThat(args["-d"]);
    }
}

} // namespace

int main(int argc, char *argv[])
try {
    Tests::runIfRequested(argc, argv, testParseArguments);

    Arguments args;
    args.addOption("-a", false).addOption("-b", false);
    args.parse(argc, argv);

    std::cout << "positional argument count: " << args.size() << '\n';
    for (const auto &arg : args)
        std::cout << "positional argument: " << arg << '\n';

    std::cout << "option a: " << args["-a"] << '\n';
    std::cout << "option b: " << args["-b"] << '\n';

    return EXIT_SUCCESS;
}
catch (std::exception &e) {
    std::fprintf(stderr, "caught exception: '%s', exiting...\n", e.what());
    return EXIT_FAILURE;
}
