/*!
 * \file
 * C++ 17 argument handling demo program
 */

#include <iostream>
#include <string_view>
#include <vector>
#include <utility>

int main(int argc, char *argv[])
{
    std::vector<std::string_view> arguments{argv, argv + argc};
    std::cout << "argc: " << arguments.size() << std::endl;
    for (const auto &arg : std::as_const(arguments))
        std::cout << "  arg: " << arg << std::endl;

    return EXIT_SUCCESS;
}
