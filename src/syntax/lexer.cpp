#include "lexer.h"

#include <cctype>

#include "util/error.h"
#include "lang.h"

Lexer::Lexer(std::string_view file, std::string_view source)
	: file(file),
	  source(source),
	  error(false)
{
}

bool Lexer::failed() const
{
	return error;
}

Token Lexer::tokenize_string_literal(std::string_view::const_iterator &it, std::string_view::const_iterator end, unsigned &line, unsigned &column)
{
	// initial quotation mark
	auto begin = ++it;
	unsigned length = 0;

	Util::FileLocation initial_quote = { line, column };

	while(it != end && *it != '"')
	{
		if(*it == '\n')
		{
			error = true;
			Util::Error(initial_quote, { line, column + 1 }, "unterminated string literal").print(file, source);

			line++;
			column = 0;
			return Token();
		}

		it++;
		length++;

		if(*it == '\t') column += 4;
		else column++;
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

Token Lexer::tokenize_operator_separator(std::string_view::const_iterator &it, std::string_view::const_iterator end, unsigned &line, unsigned &column)
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
				.location = { line, column - length + 1 }
			};
		}
	}

	return Token();
}

Token Lexer::tokenize_number_literal(std::string_view::const_iterator &it, std::string_view::const_iterator end, unsigned &line, unsigned &column)
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
		.location = { line, column - length + 1 }
	};
}

Token Lexer::tokenize_identifier_keyword(std::string_view::const_iterator &it, std::string_view::const_iterator end, unsigned &line, unsigned &column)
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
		.location = { line, column - length + 1 }
	};
}

std::vector<Token> Lexer::tokenize()
{
	std::vector<Token> tokens;

	unsigned line = 1;
	unsigned column = 0;

	const auto &end = source.cend();
	for(auto it = source.cbegin(); it != end; it++)
	{
		column++;

		// whitespace
		if(isspace(*it))
		{
			if(*it == '\n')
			{
				tokens.push_back(
				{
					.type = TokenType::Separator,
					.value = "\n",
					.location = { line, column }
				});
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
			tokens.push_back(
			{
				.type = TokenType::Separator,
				.value = "\n",
				.location = { line, column }
			});
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
			{
				error = true;
				Util::Error(token.location, "unexpected symbol '", *it, "'").print(file, source);
			}

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
			error = true;
			Util::Error({ line, column }, "unexpected symbol '", *it, "'").print(file, source);
		}
	}

	return tokens;
}
