#include "error.h"

#include <vector>
#include <sstream>
#include <string>
#include <iomanip>
#include <regex>

#include "../log.h"

namespace Util
{
	std::string CompilerError::what() noexcept
	{
		return message.str();
	}

	void CompilerError::print(std::string_view file, std::string_view source)
	{
		constexpr int radius = 1;
		std::stringstream stream;

		unsigned error_line_index = -1;

		// get lines
		stream << source.data();
		std::vector<std::string> lines;
		{
			std::string line;
			unsigned n = 0, m = 0;
			while(std::getline(stream, line))
			{
				if((n + radius + 1 >= begin.line) && (n <= begin.line + radius - 1))
				{
					lines.push_back(std::regex_replace(line, std::regex("\\t"), std::string(4, ' ')));
					if(n == begin.line - 1)
						error_line_index = m;
					m++;
				}
				n++;
			}
		}
		stream.str(std::string());
		stream.clear();

		int side_width = std::to_string(begin.line + 1 + radius).length();

		// print lines
		for(size_t i = 0; i < lines.size(); i++)
		{
			auto &line = lines[i];

			stream << "\033[34m";
			stream << std::setw(side_width);
			stream  << (i + begin.line - error_line_index) << " | ";

			if(i == error_line_index)
			{
				stream << "\033[0m" << line.substr(0, begin.column - 1);
				if(line[begin.column - 1])
				{
					stream << "\033[31;1m" << line.substr(begin.column - 1, end.column - begin.column);
					stream << "\033[0m" << line.substr(end.column - 1);
				}

				stream << "\n";

				stream << std::setw(side_width) << "";
				stream << "\033[34m |";
				stream << "\033[0m" << std::setw(begin.column) << "";
				std::string pointer(begin == end ? 1 : end.column - begin.column, '^');
				stream << "\033[31;1m" << pointer << " " << what() << "\033[0m";
			}
			else
			{
				stream << "\033[0m" << line;
			}

			line = stream.str();

			stream.str(std::string());
			stream.clear();
		}

		// print
		Logger::get().error(what());
		std::cout << " \033[1m" << file << ":" << begin.line << ":" << begin.column << "\033[0m" << std::endl;
		for(const auto &line : lines)
		{
			std::cout << line << std::endl;
		}
	}
}
