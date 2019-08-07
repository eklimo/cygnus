#include "compiler.h"

#include "log.h"
#include "parser/lexer.h"

namespace Compiler
{
	void compile(std::string_view file, std::string_view source)
	{
		Logger::get().debug("Tokenizing '", file, "'");
		auto tokens = Lexer::tokenize(source);

		Logger::get().debug("Tokens:");
		for(const auto &token : tokens)
		{
			Logger::get().debug("- ", token);
		}
	}
}
