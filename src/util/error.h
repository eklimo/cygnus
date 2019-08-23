#pragma once

#include <exception>
#include <sstream>
#include <string_view>

#include "util.h"
#include "syntax/token.h"

using Util::FileLocation;

namespace Util
{
	class Error : public std::runtime_error
	{
	public:
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
		{
		}

		Error();

		std::string what() noexcept;

		void print(std::string_view file, std::string_view source);
	private:
		const FileLocation begin, end;
		std::stringstream message;
	};
}
