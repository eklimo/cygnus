#pragma once

#include <string_view>

#include "util/util.h"

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
		Token(TokenType type, std::string_view value, Util::FileLocation loc);
		Token();

		const TokenType type;
		const std::string_view value;
		const Util::FileLocation location;

		friend std::ostream &operator<<(std::ostream &stream, const Token &token);
	};
}
