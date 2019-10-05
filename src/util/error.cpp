#include "error.h"

#include <string>
#include <iomanip>
#include <regex>
#include <stdexcept>

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
	                   arrow_padding = 2;
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

	void Error::format_single(std::stringstream &stream, const std::string &line, size_t i)
	{
		const auto s1 = line.substr(0, begin.column - 1),
		           s2 = line.substr(begin.column - 1, end.column - begin.column),
		           s3 = line.substr(end.column - 1);
		stream << s1 << bold << color1 << s2 << reset << s3;

		const auto pointer = std::string(end.column - begin.column, sym_pointer);
		add_modifier(modifiers, i, "", color1, bold, std::setw(end.column - 1), pointer, " ", normal, what(), reset);
	}

	void Error::format_multi_start(std::stringstream &stream, const std::string &line, size_t i)
	{
		const bool error_in_front = begin.column == 1;

		const auto arrow =
		    error_in_front
		    ? (sym_arrow_d + std::string(arrow_padding - 1, ' '))
		    : std::string(arrow_padding, ' ');
		const auto s1 = line.substr(0, begin.column - 1),
		           s2 = line.substr(begin.column - 1);
		stream << color1 << arrow << reset << s1 << bold << color1 << s2 << reset;

		if(!error_in_front)
		{
			const auto arrow = std::string(begin.column - 2 + arrow_padding, sym_arrow_h);
			add_modifier(modifiers, i, "", color1, " ", arrow, bold, sym_pointer, reset);
		}
	}

	void Error::format_multi_mid(std::stringstream &stream, const std::string &line, size_t i)
	{
		// within radius
		if(
		    (i > min_line_index && i <= min_line_index + inner_radius_top) ||
		    (i >= max_line_index - inner_radius_bottom && i < max_line_index))
		{
			const auto arrow = sym_arrow_v + std::string(arrow_padding - 1, ' ');
			stream << color1 << arrow << bold << line << reset;

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

	void Error::format_multi_end(std::stringstream &stream, const std::string &line, size_t i)
	{
		auto arrow = sym_arrow_v + std::string(arrow_padding - 1, ' ');
		const auto s1 = line.substr(0, end.column - 1),
		           s2 = line.substr(end.column - 1);
		stream << color1 << arrow << bold << s1 << reset << s2 << reset;

		arrow = sym_arrow_v + std::string(end.column - 3 + arrow_padding, sym_arrow_h);
		add_modifier(modifiers, i, "", color1, arrow, bold, sym_pointer, " ", normal, what(), reset);
	}

	void Error::format_line_numbers(std::stringstream &stream, std::vector<std::string> &lines, unsigned side_width)
	{
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
	}

	void Error::print(std::string_view file, std::string_view source)
	{
		if(what().empty()) return;

		// print error only
		if(begin.line <= 0 || begin.column <= 0 || end.line <= 0 || end.column <= 0)
		{
			Logger::get().error(what());
			std::cout << std::endl;
			return;
		}
		else if(end.column == 1)
		{
			throw std::out_of_range("end column shorter than line");
		}

		// get lines
		auto lines = get_relevant_lines(source, min_line_index, max_line_index);

		// format error
		std::stringstream stream;
		for(size_t i = 0; i < lines.size(); i++)
		{
			auto &line = lines[i];

			// single line
			if(min_line_index == max_line_index)
			{
				// error line
				if(i == min_line_index)
					format_single(stream, line, i);

				//normal line
				else
					stream << line;
			}

			// multiple lines
			else
			{
				// first error line
				if(i == min_line_index)
					format_multi_start(stream, line, i);

				// mid error line
				else if(i > min_line_index && i < max_line_index)
					format_multi_mid(stream, line, i);

				// last error line
				else if(i == max_line_index)
					format_multi_end(stream, line, i);

				// normal line
				else
					stream << std::string(arrow_padding, ' ') << line;
			}

			line = stream.str();
			stream.str(std::string());
			stream.clear();
		}

		// add line numbers
		unsigned side_width = std::to_string(end.line + 1 + outer_radius).length();
		for(auto &p : modifiers)
		{
			const auto width = p.second.first.length();
			if(width > side_width) side_width = width;
		}
		format_line_numbers(stream, lines, side_width);

		// print error
		Logger::get().error(what());
		std::cout << bold << " " << file << ":" << begin.line << ":" << begin.column << reset << std::endl;
		for(const auto &line : lines)
		{
			if(line != "")
				std::cout << line << std::endl;
		}
		std::cout << std::endl;
	}
}
