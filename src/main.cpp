#include <iostream>
#include <vector>
#include <string_view>

#include "cli.h"

int main(int argc, char *argv[])
{
	std::cout << "\033[1;35mCygnus Compiler\033[0m" << std::endl;

	std::vector<std::string_view> args(argv + 1, argv + argc);
	return CLI::execute(args);
}
