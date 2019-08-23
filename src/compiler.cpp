#include "compiler.h"

#include "log.h"
#include "util/print_visitor.h"
#include "syntax/lexer.h"
#include "syntax/token.h"
#include "syntax/parser.h"
#include "ast/node.h"
#include "semantic/symtable.h"

namespace Compiler
{
	void compile(std::string_view file, std::string_view source)
	{
		// lexer
		Logger::get().debug("Tokenizing '", file, "'");
		Lexer lexer(file, source);
		auto tokens = lexer.tokenize();
		if(lexer.failed()) throw Util::Error();

		Logger::get().debug("Tokens:");
		for(const auto &token : tokens)
		{
			Logger::get().debug("  ", token);
		}
		std::cout << std::endl;

		// parser
		Logger::get().debug("Parsing '", file, "'");
		Parser parser(tokens, file, source);
		auto ast = parser.parse();
		if(parser.failed()) throw Util::Error();

		Logger::get().debug("AST:");
		Util::PrintVisitor printer;
		ast->accept(printer);
		std::cout << std::endl;

		// symbol table
		Logger::get().debug("Building symbol table for '", file, "'");
		SymbolTable sym(file, source);
		ast->accept(sym);
		if(sym.failed()) throw Util::Error();
	}
}
