#pragma once

#include "util.h"
#include "syntax/token.h"
#include "ast/node.h"
#include "util/noderange.h"

#include <exception>
#include <sstream>
#include <string_view>
#include <vector>
#include <unordered_map>

using Util::FileLocation;

namespace Util
{
	class Error : public std::runtime_error
	{
	public:
		template<typename... Args>
		Error(Node *const node, Args &&... args)
			: std::runtime_error(""),
			  range(NodeRange(node)),
			  begin(range.begin),
			  end(range.end)
		{
			(message << ... << args);
		}

		template<typename... Args>
		static Error At(const Token &token, Args &&... args)
		{
			auto begin = token.location;

			FileLocation end
			(
			    token.location.line,
			    token.location.column + token.value.length() + (token.type == TokenType::String ? 2 : 0)
			);

			return Error(begin, end, std::forward<Args>(args)...);
		}

		template<typename... Args>
		static Error After(const Token &token, Args &&... args)
		{
			unsigned column = 0;
			switch(token.type)
			{
				case TokenType::Invalid:
					column = token.location.column;
					break;
				case TokenType::String:
					column = token.location.column + token.value.length() + 2;
					break;
				default:
					column = token.location.column + token.value.length();
					break;
			}

			FileLocation loc
			(
			    token.location.line,
			    column
			);

			return Error(loc, std::forward<Args>(args)...);
		}

		template<typename... Args>
		Error(FileLocation begin, FileLocation end, Args &&... args)
			: std::runtime_error(""),
			  begin(begin),
			  end(end)
		{
			(message << ... << args);
		}

		template<typename... Args>
		Error(FileLocation begin, Args &&... args)
			: std::runtime_error(""),
			  begin(begin),
			  end({ begin.line, begin.column + 1 })
		{
			(message << ... << args);
		}

		Error()
			: std::runtime_error("")
		{
		}

		std::string what() noexcept;

		void print(std::string_view file, std::string_view source);
	private:
		NodeRange range;
		const FileLocation begin, end;
		std::stringstream message;

		unsigned min_line_index,
		         max_line_index;
		std::unordered_map<unsigned, std::pair<std::string, std::string>> modifiers;

		std::vector<std::string> get_relevant_lines(std::string_view source, unsigned &min, unsigned &max);
		template<typename... Args>
		void add_modifier(std::unordered_map<unsigned, std::pair<std::string, std::string>> &modifiers, size_t i, const char *prefix, Args &&... args)
		{
			std::stringstream stream;
			(stream << ... << args);
			modifiers[i] = {prefix, stream.str()};
		}

		void format_single(std::vector<std::string> &lines);
		void format_single_error(std::stringstream &stream, const std::string &line, size_t i, unsigned shift_length);
		void format_single_normal(std::stringstream &stream, const std::string &line, size_t i, unsigned shift_length);

		void format_multi(std::vector<std::string> &lines);
		void format_multi_start(std::stringstream &stream, const std::string &line, size_t i, unsigned shift_length);
		void format_multi_mid(std::stringstream &stream, const std::string &line, size_t i, unsigned shift_length);
		void format_multi_end(std::stringstream &stream, const std::string &line, size_t i, unsigned shift_length);
		void format_multi_normal(std::stringstream &stream, const std::string &line, size_t i, unsigned shift_length);

		void format_line_numbers(std::vector<std::string> &lines);
	};
}
