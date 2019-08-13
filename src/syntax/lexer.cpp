#include "lexer.h"

#include <cctype>

#include "../log.h"
#include "../util/error.h"
#include "lang.h"

using Lexer::Token;
using Lexer::TokenType;

namespace Lexer
{
	Token tokenize_string_literal(std::string_view::const_iterator &it, std::string_view::const_iterator end, unsigned &line, unsigned &column)
	{
		// initial quotation mark
		auto begin = ++it;
		unsigned length = 0;

		Util::FileLocation initial_quote = { line, column };

		while(it != end && *it != '"')
		{
			it++;
			length++;

			if(*it == '\t') column += 4;
			else column++;

			if(*it == '\n' || it == end)
			{
				throw Util::CompilerError(initial_quote, { line, column + 1 }, "unterminated string literal");
			}
		}
		// closing quotation mark
		column++;

		return
		{
			.type = TokenType::String,
			.value = std::string_view(begin, length),
			.location = initial_quote
		};
	}

	Token tokenize_operator_separator(std::string_view::const_iterator &it, std::string_view::const_iterator end, unsigned &line, unsigned &column)
	{
		// maximum length of any operator or separator
		constexpr int max_length = 2;

		for(unsigned length = max_length; length >= 1; length--)
		{
			std::string_view ahead(it, length);
			bool valid = false;
			auto type = TokenType::Invalid;

			if(Lang::is_operator(ahead))
			{
				valid = true;
				type = TokenType::Operator;
			}
			else if(Lang::is_separator(ahead))
			{
				valid = true;
				type = TokenType::Separator;
			}

			if(valid)
			{
				it += length - 1;
				column += length - 1;
				return
				{
					.type = type,
					.value = ahead,
					.location = { line, column }
				};
			}
		}

		return
		{
			.type = TokenType::Invalid,
			.value = "",
			.location = { line, column }
		};
	}

	Token tokenize_number_literal(std::string_view::const_iterator &it, std::string_view::const_iterator end, unsigned &line, unsigned &column)
	{
		auto begin = it;
		unsigned length = 0;

		while(it != end && isdigit(*it))
		{
			it++;
			length++;
			column++;
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
			.location = { line, column }
		};
	}

	Token tokenize_identifier_keyword(std::string_view::const_iterator &it, std::string_view::const_iterator end, unsigned &line, unsigned &column)
	{
		auto begin = it;
		unsigned length = 0;

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
			.location =
			{
				.line = line,
				.column = column - length + 1
			}
		};
	}

	std::vector<Token> tokenize(std::string_view input)
	{
		std::vector<Token> tokens;

		unsigned line = 1;
		unsigned column = 0;

		const auto &end = input.cend();
		for(auto it = input.cbegin(); it != end; it++)
		{
			column++;

			// whitespace
			if(isspace(*it))
			{
				if(*it == '\n')
				{
					line++;
					column = 0;
				}
				else if(*it == '\t')
				{
					column += 3;
				}
				continue;
			}

			// comment
			else if(*it == '#')
			{
				while(it != end && *it != '\n')
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
				tokens.push_back(tokenize_string_literal(it, end, line, column));
			}

			// operator/separator
			else if(ispunct(*it) && *it != '_')
			{
				auto token = tokenize_operator_separator(it, end, line, column);
				if(token.type == TokenType::Invalid)
					throw Util::CompilerError(token.location, "unexpected symbol '", *it, "'");

				tokens.push_back(token);
			}

			// number literal
			else if(isdigit(*it))
			{
				tokens.push_back(tokenize_number_literal(it, end, line, column));
			}

			// identifier/keyword/word operator
			else if(isalpha(*it) || *it == '_')
			{
				tokens.push_back(tokenize_identifier_keyword(it, end, line, column));
			}

			// invalid
			else
			{
				throw Util::CompilerError({ line, column }, "unexpected symbol '", *it, "'");
			}
		}

		return tokens;
	}
}
