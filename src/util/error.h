#pragma once

#include <exception>
#include <sstream>

namespace Util
{
	class CompilerError : public std::runtime_error
	{
	public:
		template<typename... Args>
		CompilerError(Args &&... args)
			: std::runtime_error("")
		{
			(message << ... << args);
		}
		std::string what() throw();
	private:
		std::stringstream message;

	};
}
