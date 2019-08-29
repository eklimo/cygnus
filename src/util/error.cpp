#include "error.h"

#include <string>
#include <iomanip>
#include <regex>
#include <stdexcept>

#include "log.h"

namespace Util
{
	Error::Error()
		: std::runtime_error(""),
		  begin({ 0, 0 }),
	end({ 0, 0 }),
	message("")
	{
	}

	std::string Error::what() noexcept
	{
		return message.str();
	}

	void Error::print(std::string_view file, std::string_view source)
	{
		if(what().empty()) return;

		// print error only
		if(begin.line == 0 || begin.column == 0 || end.line == 0 || end.column == 0)
		{
			Logger::get().error(what());
			std::cout << std::endl;
			return;
		}

		std::stringstream stream;

		constexpr int radius = 1;
		const char pointer_char = '^';

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
					// standardize tab size
					lines.push_back(std::regex_replace(line, std::regex("\\t"), std::string(4, ' ')));
					if(n == begin.line - 1)
					{
						error_line_index = m;
						if(end.column > line.length() + 1)
							throw std::out_of_range("column longer than line");
					}
					m++;
				}
				n++;
			}
		}
		stream.str(std::string());
		stream.clear();

		// format
		int side_width = std::to_string(begin.line + 1 + radius).length();
		for(size_t i = 0; i < lines.size(); i++)
		{
			auto &line = lines[i];

			stream << "\033[34m";
			stream << std::setw(side_width);
			stream << (i + begin.line - error_line_index) << " | ";

			if(i == error_line_index)
			{
				stream << "\033[0m" << line.substr(0, begin.column - 1);
				if(line[begin.column - 1])
				{
					stream << "\033[1;31m" << line.substr(begin.column - 1, end.column - begin.column);
					stream << "\033[0m" << line.substr(end.column - 1);
				}

				stream << "\n";

				stream << std::setw(side_width) << "";
				stream << "\033[34m |";
				stream << "\033[0m" << std::setw(begin.column) << "";
				int length = begin == end ? 1 : (int)end.column - begin.column;
				std::string pointer(length, pointer_char);
				stream << "\033[1;31m" << pointer << " \033[0;31m" << what() << "\033[0m";
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
		std::cout << std::endl;
	}
}
