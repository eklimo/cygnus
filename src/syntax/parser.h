#pragma once

#include <vector>
#include <memory>
#include <string_view>
#include <exception>
#include <sstream>

#include "../ast/node.h"
#include "lexer.h"
#include "token.h"

using Lexer::Token;
using Lexer::TokenType;

class Parser
{
public:
	explicit Parser(std::vector<Token> &tokens);
	std::unique_ptr<Node> parse();

	class ParseError : public std::runtime_error
	{
	public:
		template<typename... Args>
		ParseError(Args &&... args)
			: std::runtime_error("")
		{
			(message << ... << args);
		}
		std::string what() throw();
	private:
		std::stringstream message;
	};

private:
	std::vector<Token>::const_iterator it;
	const std::vector<Token>::const_iterator end;

	const Token &token() const;
	bool advance();
	bool match(TokenType type);
	bool match(std::string_view value);
	bool match(TokenType type, std::string_view value);

	std::unique_ptr<Node> expression(int rbp = 0);
	std::unique_ptr<Node> null_denotation(const Token &token);
	std::unique_ptr<Node> left_denotation(const Token &token, std::unique_ptr<Node> &left);
};
