#include "util.h"

#include <fstream>
#include <sstream>

#include "log.h"

namespace Util
{
	std::string read_file(std::string_view path)
	{
		std::ifstream file;
		file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		file.open(path.data());

		std::stringstream buffer;
		buffer << file.rdbuf();
		return buffer.str();
	}
}
