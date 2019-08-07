#include "lexer.h"

#include <ostream>
#include <cctype>
#include <functional>

#include "log.h"

namespace Lexer
{
	constexpr std::string_view keywords[] = {"var", "true", "false"};
	// descending length
	constexpr std::string_view operators[] = {"==", "++", "--", "=", "+", "-", "*", "/", "."};
	constexpr std::string_view word_operators[] = {"and", "or"};
	constexpr std::string_view separators[] = {"(", ")", "[", "]", "{", "}", ","};

	constexpr bool is_keyword(std::string_view str)
	{
		for(const auto &keyword : keywords)
		{
			if(str == keyword)
			{
				return true;
			}
		}
		return false;
	}
	constexpr bool is_operator(std::string_view str)
	{
		for(const auto &op : operators)
		{
			if(str == op)
			{
				return true;
			}
		}
		return false;
	}
	constexpr bool is_word_operator(std::string_view str)
	{
		for(const auto &op : word_operators)
		{
			if(str == op)
			{
				return true;
			}
		}
		return false;
	}
	constexpr bool is_separator(std::string_view str)
	{
		for(const auto &sep : separators)
		{
			if(str == sep)
			{
				return true;
			}
		}
		return false;
	}

	Token tokenize_string_literal(std::string_view::const_iterator &it, std::string_view::const_iterator end, unsigned &line, unsigned &column)
	{
		// skip initial quotation mark
		auto begin = ++it;
		std::string_view::size_type length = 0;

		while(it != end && *it != '"')
		{
			it++;
			length++;
			column++;
		}
		// skip closing quotation mark
		// it--;
		// column--;

		return
		{
			.type = TokenType::String,
			.value = std::string_view(begin, length),
			.line = line,
			.column = column
		};
	}

	Token tokenize_operator_separator(std::string_view::const_iterator &it, std::string_view::const_iterator end, unsigned &line, unsigned &column)
	{
		for(const auto &op : operators)
		{
			auto length = op.length();
			std::string_view ahead(it, length);
			if(is_operator(ahead))
			{
				it += length - 1;
				column += length - 1;
				return
				{
					.type = TokenType::Operator,
					.value = ahead,
					.line = line,
					.column = column
				};
			}
		}

		for(const auto &sep : separators)
		{
			auto length = sep.length();
			std::string_view ahead(it, length);
			if(is_separator(ahead))
			{
				it += length - 1;
				column += length - 1;
				return
				{
					.type = TokenType::Separator,
					.value = ahead,
					.line = line,
					.column = column
				};
			}
		}

		return {};
	}

	Token tokenize_number_literal(std::string_view::const_iterator &it, std::string_view::const_iterator end, unsigned &line, unsigned &column)
	{
		auto begin = it;
		std::string_view::size_type length = 0;

		while(it != end && isdigit(*it))
		{
			it++;
			length++;
		}
		if(*it == '.')
		{
			it++;
			length++;
			column++;
			while(it != end && isdigit(*it))
			{
				it++;
				length++;
				column++;
			}
		}
		it--;
		column--;

		return
		{
			.type = TokenType::Number,
			.value = std::string_view(begin, length),
			.line = line,
			.column = column
		};
	}

	Token tokenize_identifier_keyword(std::string_view::const_iterator &it, std::string_view::const_iterator end, unsigned &line, unsigned &column)
	{
		auto begin = it;
		std::string_view::size_type length = 0;

		while(it != end && (isalnum(*it) || *it == '_'))
		{
			it++;
			length++;
			column++;
		}
		it--;
		column--;

		Token token =
		{
			.type = TokenType::Identifier,
			.value = std::string_view(begin, length),
			.line = line,
			.column = column
		};

		if(is_keyword(token.value))
		{
			token.type = TokenType::Keyword;
		}
		else if(is_word_operator(token.value))
		{
			token.type = TokenType::Operator;
		}

		return token;
	}

	std::vector<Token> tokenize(std::string_view input)
	{
		std::vector<Token> tokens;
		bool invalid = false;

		unsigned line = 1;
		unsigned column = 1;

		for(auto it = input.cbegin(); it != input.cend(); it++)
		{
			column++;

			// whitespace
			if(isspace(*it))
			{
				if(it != input.end() && *it == '\n')
				{
					line++;
					column = 0;
				}
				continue;
			}

			// comment
			else if(*it == '#')
			{
				while(it != input.end() && *it != '\n')
				{
					it++;
					column++;
				}
				line++;
				column = 0;
			}

			// string literal
			else if(*it == '"')
			{
				tokens.push_back(tokenize_string_literal(it, input.end(), line, column));
			}

			// operator/separator
			else if(ispunct(*it) && *it != '_')
			{
				auto token = tokenize_operator_separator(it, input.end(), line, column);
				if(token.type == TokenType::Invalid)
				{
					Logger::get().error("Invalid symbol '", *it, "' at ", line, ":", column);
					invalid = true;
				}
				tokens.push_back(token);
			}

			// number literal
			else if(isdigit(*it))
			{
				tokens.push_back(tokenize_number_literal(it, input.end(), line, column));
			}

			// identifier/keyword/word operator
			else if(isalpha(*it) || *it == '_')
			{
				tokens.push_back(tokenize_identifier_keyword(it, input.end(), line, column));
			}

			// invalid
			else
			{
				Logger::get().error("Invalid symbol '", *it, "' at ", line, ":", column);
				tokens.push_back(
				{
					.type = TokenType::Invalid,
					.value = std::string_view(it, 1),
					.line = line,
					.column = column
				});
				invalid = true;
				continue;
			}
		}

		if(invalid)
		{
			std::exit(0);
		}

		return tokens;
	}

	constexpr const char *type_to_string(TokenType type)
	{
		switch(type)
		{
			case TokenType::Invalid:
				return "invalid";
			case TokenType::Number:
				return "number";
			case TokenType::String:
				return "string";
			case TokenType::Identifier:
				return "identifier";
			case TokenType::Keyword:
				return "keyword";
			case TokenType::Operator:
				return "operator";
			case TokenType::Separator:
				return "separator";
			default:
				return "";
		}
	}

	std::ostream &operator<<(std::ostream &stream, const Token &token)
	{
		return stream << "[" << type_to_string(token.type) << "] '" << token.value << "'";
	}
}
