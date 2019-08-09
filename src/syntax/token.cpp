#include "token.h"

#include <ostream>

namespace Lexer
{
	Token::Token(TokenType _type, std::string_view _value, unsigned _line, unsigned _column)
		: type(_type), value(_value), line(_line), column(_column)
	{}

	Token::Token()
		: type(TokenType::Invalid), line(0), column(0)
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
				return "id";
			case TokenType::Keyword:
				return "key";
			case TokenType::Operator:
				return "op";
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
