#pragma once

#include <vector>
#include <string_view>

namespace Lexer
{
	enum class TokenType
	{
		Invalid,
		Number,
		String,
		Identifier,
		Keyword,
		Operator,
		Separator
	};

	struct Token
	{
		TokenType type = TokenType::Invalid;
		std::string_view value;
		unsigned line, column;

		friend std::ostream &operator<<(std::ostream &stream, const Token &token);
	};

	std::vector<Token> tokenize(std::string_view input);
}
