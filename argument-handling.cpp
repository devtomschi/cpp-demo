/*!
 * \file
 * C++ 17 argument handling demo program
 */

#include <cstdio>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <string_view>
#include <utility>
#include <vector>

int main(int argc, char *argv[])
try {
    std::vector<std::string_view> arguments{argv, argv + argc};
    std::cout << "argc: " << arguments.size() << '\n';
    for (const auto &arg : std::as_const(arguments))
        std::cout << "arg: " << arg << '\n';

    return EXIT_SUCCESS;
}
catch (std::exception &e) {
    std::fprintf(stderr, "caught exception: '%s', exiting...\n", e.what());
    return EXIT_FAILURE;
}
