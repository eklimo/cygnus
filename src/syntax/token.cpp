#include "token.h"

#include <ostream>

namespace Lexer
{
	Token::Token(TokenType _type, std::string_view _value, Util::FileLocation _location)
		: type(_type),
		  value(_value),
		  location(_location)
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
		return stream << "[" << type_to_string(token.type) << "] " << token.value;
	}
}
