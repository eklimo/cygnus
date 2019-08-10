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
	explicit Parser(std::vector<Token> &tokens);
	std::unique_ptr<Node> parse();

private:
	std::vector<Token>::const_iterator it;
	const std::vector<Token>::const_iterator begin, end;
	Util::StringifyVisitor stringifier;

	const Token &token() const;
	bool advance();
	bool is_valid_index(int n) const;
	bool match(TokenType type);
	bool match(std::string_view value);
	bool match(TokenType type, std::string_view value);

	std::unique_ptr<Node> expression(int rbp = 0);
	std::unique_ptr<Node> null_denotation(const Token &token);
	std::unique_ptr<Node> left_denotation(const Token &token, std::unique_ptr<Node> &left);
};
