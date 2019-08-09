#pragma once

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

	class Token
	{
	public:
		Token(TokenType type, std::string_view value, unsigned line, unsigned column);
		Token();

		const TokenType type;
		const std::string_view value;
		const unsigned line, column;

		friend std::ostream &operator<<(std::ostream &stream, const Token &token);
	};
}
