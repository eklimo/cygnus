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

	FileLocation::FileLocation(unsigned line, unsigned column)
		: line(line), column(column)
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

	bool FileLocation::operator>(const FileLocation &rhs) const
	{
		return line > rhs.line || (line == rhs.line && column > rhs.column);
	}
	bool FileLocation::operator>=(const FileLocation &rhs) const
	{
		return line >= rhs.line || (line == rhs.line && column >= rhs.column);
	}
	bool FileLocation::operator<(const FileLocation &rhs) const
	{
		return line < rhs.line || (line == rhs.line && column < rhs.column);
	}
	bool FileLocation::operator<=(const FileLocation &rhs) const
	{
		return line <= rhs.line || (line == rhs.line && column <= rhs.column);
	}

	std::ostream &operator<<(std::ostream &stream, const FileLocation &loc)
	{
		return stream << loc.line << ":" << loc.column;
	}
}
