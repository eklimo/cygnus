#pragma once

#include <string>
#include <string_view>

namespace Util
{
	std::string read_file(std::string_view path);

	struct FileLocation
	{
		FileLocation(unsigned line = 0, unsigned column = 0);

		unsigned line;
		unsigned column;

		bool operator==(const FileLocation &rhs) const;
		bool operator!=(const FileLocation &rhs) const;
		bool operator>(const FileLocation &rhs) const;
		bool operator>=(const FileLocation &rhs) const;
		bool operator<(const FileLocation &rhs) const;
		bool operator<=(const FileLocation &rhs) const;

		friend std::ostream &operator<<(std::ostream &stream, const FileLocation &loc);
	};
}
