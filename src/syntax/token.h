#pragma once

#include "util/util.h"

#include <string_view>

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
