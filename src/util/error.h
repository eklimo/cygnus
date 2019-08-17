#pragma once

#include <exception>
#include <sstream>
#include <string_view>
#include <vector>

#include "util.h"
#include "../syntax/token.h"

using Util::FileLocation;

namespace Util
{
	class Error : public std::runtime_error
	{
	public:
		template<typename... Args>
		static Error At(const std::vector<Lexer::Token>::const_iterator it, Args &&... args)
		{
			if(it->type == Lexer::TokenType::Invalid)
				return After(it - 1, std::forward<Args>(args)...);

			auto begin = it->location;

			FileLocation end
			(
			    it->location.line,
			    it->location.column + it->value.length() + (it->type == Lexer::TokenType::String ? 2 : 0)
			);

			return Error(begin, end, std::forward<Args>(args)...);
		}

		template<typename... Args>
		static Error After(const std::vector<Lexer::Token>::const_iterator it, Args &&... args)
		{
			unsigned column  = 0;
			switch(it->type)
			{
				case Lexer::TokenType::Invalid:
					column = it->location.column;
					break;
				case Lexer::TokenType::String:
					column = it->location.column + it->value.length() + 2;
					break;
				default:
					column = it->location.column + it->value.length();
					break;
			}

			FileLocation loc
			(
			    it->location.line,
			    column
			);

			return Error(loc, std::forward<Args>(args)...);
		}

		template<typename... Args>
		Error(FileLocation _begin, FileLocation _end, Args &&... args)
			:  std::runtime_error(""),
			   begin(_begin),
			   end(_end)
		{
			(message << ... << args);
		}

		template<typename... Args>
		Error(FileLocation _begin, Args &&... args)
			: Error(_begin,
		{
			.line = _begin.line,
			.column = _begin.column + 1
		}, std::forward<Args>(args)...)
		{}

		std::string what() noexcept;

		void print(std::string_view file, std::string_view source);
	private:
		const FileLocation begin, end;
		std::stringstream message;
	};
}
