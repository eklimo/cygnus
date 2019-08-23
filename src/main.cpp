#include <vector>
#include <string_view>

#include "cli.h"

int main(int argc, char *argv[])
{
	std::vector<std::string_view> args(argv + 1, argv + argc);
	CLI::execute(args);

	return 0;
}
