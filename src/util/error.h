#pragma once

#include <exception>
#include <sstream>
#include <string_view>

#include "util.h"

namespace Util
{
	class CompilerError : public std::runtime_error
	{
	public:
		template<typename... Args>
		CompilerError(Util::FileLocation _begin, Util::FileLocation _end, Args &&... args)
			:  std::runtime_error(""),
			   begin(_begin),
			   end(_end)
		{
			(message << ... << args);
		}

		template<typename... Args>
		CompilerError(Util::FileLocation _begin, Args &&... args)
			: CompilerError(_begin,
		{
			.line = _begin.line,
			.column = _begin.column + 1
		}, std::forward<Args>(args)...)
		{}

		std::string what() noexcept;

		void print(std::string_view file, std::string_view source);
	private:
		const Util::FileLocation begin, end;
		std::stringstream message;
	};
}
