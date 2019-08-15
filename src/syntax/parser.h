#pragma once

#include <vector>
#include <memory>
#include <string_view>
#include <exception>
#include <sstream>

#include "../ast/node.h"
#include "../util/error.h"
#include "../util/stringify_visitor.h"
#include "lexer.h"
#include "token.h"

using Lexer::Token;
using Lexer::TokenType;

class Parser
{
public:
	explicit Parser(const std::vector<Token> &tokens);
	std::unique_ptr<Program> parse();

private:
	std::vector<Token>::const_iterator it;
	const std::vector<Token>::const_iterator begin, end;

	const Token &token() const;
	bool advance();
	bool is_valid_index(int n) const;
	bool match(TokenType type);
	bool match(std::string_view value);
	bool match(TokenType type, std::string_view value);

	// expressions
	std::unique_ptr<Expression> expression(int rbp = 0);
	std::unique_ptr<Expression> null_denotation(const Token &token);
	std::unique_ptr<Expression> left_denotation(const Token &token, std::unique_ptr<Expression> &left);

	// statements
	std::unique_ptr<Program> program();
	std::unique_ptr<Statement> statement();
	std::unique_ptr<VariableDef> variable_definition();

	// general
};
