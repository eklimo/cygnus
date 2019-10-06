#include "error.h"

#include <string>
#include <iomanip>
#include <regex>
#include <stdexcept>
#include <climits>

#include "log.h"

namespace Util
{
	std::string Error::what() noexcept
	{
		return message.str();
	}

	constexpr unsigned outer_radius = 1,
	                   inner_radius_top = 3,
	                   inner_radius_bottom = 1,
	                   arrow_padding = 3;
	constexpr char sym_pointer = '^',
	               sym_arrow_h = '_',
	               sym_arrow_v = '|',
	               sym_arrow_d = '/',
	               sym_separator = '|';

	constexpr auto reset = "\033[0m";
	constexpr auto normal = "\033[22m";
	constexpr auto bold = "\033[1m";
	constexpr auto color1 = "\033[31m";
	constexpr auto color2 = "\033[34m";

	unsigned count_spaces(const std::string &str)
	{
		unsigned spaces = 0;
		while(isspace(str[spaces++]));
		return --spaces;
	}

	std::pair<unsigned, unsigned> subtract(unsigned a, unsigned b)
	{
		if(a >= b) return {a - b, 0};
		else return {0, b - a};
	}

	std::vector<std::string> Error::get_relevant_lines(std::string_view source, unsigned &min, unsigned &max)
	{
		std::vector<std::string> lines;

		std::stringstream stream;
		stream << source.data();

		std::string line;
		unsigned n = 0, m = 0;
		while(std::getline(stream, line))
		{
			if((n + outer_radius + 1 >= begin.line) && (n <= end.line + outer_radius - 1))
			{
				// standardize tab size
				line = std::regex_replace(line, std::regex("\\t"), std::string(4, ' '));

				lines.push_back(line);
				if(n == begin.line - 1)
				{
					min = m;
					if(begin.column > line.length() + 1)
						throw std::out_of_range("begin column longer than line");
				}
				if(n == end.line - 1)
				{
					max = m;
					if(end.column > line.length() + 1)
						throw std::out_of_range("end column longer than line");
				}
				m++;
			}
			n++;
		}

		return lines;
	}

	// single line

	void Error::format_single(std::vector<std::string> &lines)
	{
		std::stringstream stream;

		unsigned outer_space = UINT_MAX;
		for(size_t i = 0; i < lines.size(); i++)
		{
			if(lines[i].empty()) continue;
			const unsigned spaces = count_spaces(lines[i]);
			if(spaces < outer_space) outer_space = spaces;
		}

		unsigned inner_space = UINT_MAX;
		if(!lines[min_line_index].empty())
		{
			const unsigned spaces = count_spaces(lines[min_line_index]);
			if(spaces < inner_space) inner_space = spaces;
		}
		else inner_space = outer_space;

		auto shift_length = outer_space;
		if(shift_length > inner_space) shift_length = inner_space;

		for(size_t i = 0; i < lines.size(); i++)
		{
			auto &line = lines[i];

			if(min_line_index == max_line_index)
			{
				// error line
				if(i == min_line_index)
					format_single_error(stream, line, i, shift_length);

				//normal line
				else
					format_single_normal(stream, line, i, shift_length);
			}

			line = stream.str();
			stream.str(std::string());
			stream.clear();
		}
	}

	void Error::format_single_error(std::stringstream &stream, const std::string &line, size_t i, unsigned shift_length)
	{
		if(line != "")
		{
			const auto str1 = line.substr(0, begin.column - 1).substr(shift_length),
			           str2 = line.substr(begin.column - 1, end.column - begin.column),
			           str3 = line.substr(end.column - 1);
			stream << str1 << bold << color1 << str2 << reset << str3;
		}

		const auto pointer = std::string(end.column - begin.column, sym_pointer);
		add_modifier(modifiers, i, "", color1, std::setw(end.column - 1 - shift_length), pointer, " ", normal, what(), reset);
	}

	void Error::format_single_normal(std::stringstream &stream, const std::string &line, size_t i, unsigned shift_length)
	{
		if(line != "")
			stream << line.substr(shift_length);
	}

	// multiline

	void Error::format_multi(std::vector<std::string> &lines)
	{
		std::stringstream stream;

		unsigned outer_space = UINT_MAX;
		for(size_t i = 0; i < lines.size(); i++)
		{
			if(lines[i].empty()) continue;
			const unsigned spaces = count_spaces(lines[i]);
			if(spaces < outer_space) outer_space = spaces;
		}

		unsigned inner_space = UINT_MAX;
		for(size_t i = min_line_index; i <= max_line_index; i++)
		{
			if(lines[i].empty()) continue;
			const unsigned spaces = count_spaces(lines[i]);
			if(spaces < inner_space) inner_space = spaces;
		}

		auto shift_length = 1 + arrow_padding + outer_space;
		if(shift_length > inner_space) shift_length = inner_space;

		for(size_t i = 0; i < lines.size(); i++)
		{
			auto &line = lines[i];

			// first error line
			if(i == min_line_index)
				format_multi_start(stream, line, i, shift_length);

			// mid error line
			else if(i > min_line_index && i < max_line_index)
				format_multi_mid(stream, line, i, shift_length);

			// last error line
			else if(i == max_line_index)
				format_multi_end(stream, line, i, shift_length);

			// normal line
			else
				format_multi_normal(stream, line, i, shift_length);

			line = stream.str();
			stream.str(std::string());
			stream.clear();
		}
	}

	void Error::format_multi_start(std::stringstream &stream, const std::string &line, size_t i, unsigned shift_length)
	{
		const auto sub = subtract(arrow_padding, shift_length);

		const bool error_in_front = begin.column <= 1 + count_spaces(line); // begin.column - spaces <= 1

		if(error_in_front)
		{
			const auto arrow = sym_arrow_d + std::string(sub.first, ' ');
			if(line != "")
			{
				const auto str = line.substr(sub.second);
				stream << color1 << arrow << bold << color1 << str << reset;
			}
			else
			{
				stream << color1 << arrow;
			}
		}
		else
		{
			if(line != "")
			{
				const auto padding = std::string(1 + sub.first, ' ');
				if(line != "")
				{
					const auto str1 = line.substr(0, begin.column - 1).substr(sub.second),
					           str2 = line.substr(begin.column - 1);
					stream << color1 << padding << reset << str1 << bold << color1 << str2 << reset;
				}
			}

			const auto arrow = " " + std::string(begin.column - 1 + sub.first - sub.second, sym_arrow_h);
			add_modifier(modifiers, i, "", color1, arrow, sym_pointer, reset);
		}
	}

	void Error::format_multi_mid(std::stringstream &stream, const std::string &line, size_t i, unsigned shift_length)
	{
		const auto sub = subtract(arrow_padding, shift_length);

		// within radius
		if(
		    (i > min_line_index && i <= min_line_index + inner_radius_top) ||
		    (i >= max_line_index - inner_radius_bottom && i < max_line_index))
		{
			const auto arrow = sym_arrow_v + std::string(sub.first, ' ');
			if(line != "")
			{
				const auto str = line.substr(sub.second);
				stream << color1 << arrow << bold << str << reset;
			}
			else
			{
				stream << color1 << arrow;
			}

			if(i == min_line_index + inner_radius_top)
			{
				add_modifier(modifiers, i, "...", color1, sym_arrow_v, reset);
			}
		}
		// outside of radius
		else if(i > min_line_index + inner_radius_top && i < max_line_index - inner_radius_bottom)
		{
			add_modifier(modifiers, i, "\b", "");
		}
	}

	void Error::format_multi_end(std::stringstream &stream, const std::string &line, size_t i, unsigned shift_length)
	{
		const auto sub = subtract(arrow_padding, shift_length);

		auto arrow = sym_arrow_v + std::string(sub.first, ' ');
		if(line != "")
		{
			const auto str1 = line.substr(0, end.column - 1).substr(sub.second),
			           str2 = line.substr(end.column - 1);
			stream << color1 << arrow << bold << str1 << reset << str2 << reset;
		}
		else
		{
			stream << color1 << arrow << reset;
		}

		arrow = sym_arrow_v + std::string(end.column - 2 + sub.first - sub.second, sym_arrow_h);
		add_modifier(modifiers, i, "", color1, arrow, sym_pointer, " ", normal, what(), reset);

	}

	void Error::format_multi_normal(std::stringstream &stream, const std::string &line, size_t i, unsigned shift_length)
	{
		if(line != "")
		{
			const auto sub = subtract(1 + arrow_padding, shift_length);
			const auto str = line.substr(sub.second);
			stream << std::string(sub.first, ' ') << str;
		}
	}

	void Error::format_line_numbers(std::vector<std::string> &lines)
	{
		std::stringstream stream;

		unsigned side_width = std::to_string(end.line + 1 + outer_radius).length();
		for(auto &p : modifiers)
		{
			const auto width = p.second.first.length();
			if(width > side_width) side_width = width;
		}

		for(size_t i = 0; i < lines.size(); i++)
		{
			auto &line = lines[i];

			if(modifiers[i].first != "\b")
			{
				const auto line_number = (i + begin.line - min_line_index);
				stream << color2 << std::setw(side_width) << line_number;
				stream << " " << sym_separator << " " << reset << line;

				// special line
				if(modifiers.find(i) != modifiers.end() && modifiers[i].second != "")
				{
					const auto &prefix = modifiers[i].first;
					const auto &line = modifiers[i].second;

					stream << "\n";
					stream << color2 << std::setw(side_width) << prefix;
					stream << " " << sym_separator << " " << reset << line;
				}
			}

			line = stream.str();
			stream.str(std::string());
			stream.clear();
		}

		stream << color2 << std::setw(side_width + 2) << sym_separator << reset;
		lines.insert(lines.begin(), stream.str());
		stream.str(std::string());
		stream.clear();

		stream << color2 << std::setw(side_width + 2) << sym_separator << reset;
		lines.push_back(stream.str());
		stream.str(std::string());
		stream.clear();
	}

	void Error::print(std::string_view file, std::string_view source)
	{
		if(what().empty()) return;

		// catch special cases
		if((begin.line <= 0 && begin.column <= 0) || (end.line <= 0 && end.column <= 0))
		{
			Logger::get().error(what());
			std::cout << std::endl;
			return;
		}
		else if(begin.line <= 0 || begin.column <= 0 || end.line <= 0 || end.column <= 0)
		{
			throw std::out_of_range("line or column is 0");
		}
		else if(end.column == 1)
		{
			throw std::out_of_range("end column shorter than line");
		}

		// 1. get lines
		auto lines = get_relevant_lines(source, min_line_index, max_line_index);

		// 2. format error
		if(min_line_index == max_line_index)
			format_single(lines);
		else
			format_multi(lines);

		// 3. add line numbers
		format_line_numbers(lines);

		// 4. print error
		Logger::get().error(what());
		std::cout << bold << " " << file << ":" << begin.line << ":" << begin.column << reset << std::endl;
		for(const auto &line : lines)
		{
			std::cout << line << std::endl;
		}
		std::cout << std::endl;
	}
}
