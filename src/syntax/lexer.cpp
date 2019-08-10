#include "lexer.h"

#include <cctype>

#include "log.h"
#include "util/error.h"
#include "lang.h"

using Lexer::Token;
using Lexer::TokenType;

namespace Lexer
{
	using LexerError = Util::CompilerError;

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
		for(const auto &op : Lang::operators)
		{
			auto length = op.length();
			std::string_view ahead(it, length);
			if(Lang::is_operator(ahead))
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

		for(const auto &sep : Lang::separators)
		{
			auto length = sep.length();
			std::string_view ahead(it, length);
			if(Lang::is_separator(ahead))
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

		return
		{
			.type = TokenType::Invalid,
			.value = std::string_view(it, 0),
			.line = line,
			.column = column
		};
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

		auto type = TokenType::Identifier;
		auto value = std::string_view(begin, length);


		if(Lang::is_keyword(value))
		{
			type = TokenType::Keyword;
		}
		else if(Lang::is_word_operator(value))
		{
			type = TokenType::Operator;
		}

		return
		{
			.type = type,
			.value = value,
			.line = line,
			.column = column
		};
	}

	std::vector<Token> tokenize(std::string_view input)
	{
		std::vector<Token> tokens;

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
					throw LexerError("Unexpected symbol '", *it, "'");

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
				throw LexerError("Unexpected symbol '", *it, "'");
			}
		}

		return tokens;
	}
}
