#include "util.h"

#include <fstream>
#include <sstream>

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

	FileLocation::FileLocation(unsigned _line, unsigned _column)
		: line(_line), column(_column)
	{
	}

	bool FileLocation::operator==(const FileLocation &rhs) const
	{
		return line == rhs.line && column == rhs.column;
	}

	bool FileLocation::operator!=(const FileLocation &rhs) const
	{
		return !(*this == rhs);
	}
}
