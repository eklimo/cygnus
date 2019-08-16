#include "compiler.h"

#include "log.h"
#include "util/print_visitor.h"
#include "syntax/lexer.h"
#include "syntax/token.h"
#include "syntax/parser.h"
#include "ast/node.h"

namespace Compiler
{
	void compile(std::string_view file, std::string_view source)
	{
		// lexer
		Logger::get().debug("Tokenizing '", file, "'");
		std::vector<Lexer::Token> tokens;
		try
		{
			tokens = Lexer::tokenize(source);
		}
		catch(Util::Error &e)
		{
			e.print(file, source);
			std::exit(0);
		}

		Logger::get().debug("Tokens:");
		for(const auto &token : tokens)
		{
			Logger::get().debug("  ", token);
		}

		// parser
		Logger::get().debug("Parsing '", file, "'");
		Parser parser(tokens);
		std::unique_ptr<Node> ast;
		try
		{
			ast = parser.parse();
		}
		catch(Util::Error &e)
		{
			e.print(file, source);
			std::exit(0);
		}
		Util::PrintVisitor printer;
		Logger::get().debug("AST:");
		ast->accept(printer);
	}
}
