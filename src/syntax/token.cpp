#include "token.h"

#include <ostream>
#include <regex>

Token::Token(TokenType type, std::string_view value, Util::FileLocation location)
	: type(type),
	  value(value),
	  location(location)
{}

Token::Token()
	: type(TokenType::Invalid),
	  value(""),
	  location({})
{}

constexpr const char *type_to_string(TokenType type)
{
	switch(type)
	{
		case TokenType::Invalid:
			return "inv";
		case TokenType::Number:
			return "num";
		case TokenType::String:
			return "str";
		case TokenType::Identifier:
			return "idn";
		case TokenType::Keyword:
			return "key";
		case TokenType::Operator:
			return "opr";
		case TokenType::Separator:
			return "sep";
		default:
			return "";
	}
}

std::ostream &operator<<(std::ostream &stream, const Token &token)
{
	return stream << "[" << type_to_string(token.type) << "] " << std::regex_replace(std::string(token.value), std::regex("\n"), "\\n");
}
