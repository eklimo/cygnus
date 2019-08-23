#pragma once

#include <string_view>
#include <vector>

#include "token.h"

class Lexer
{
public:
	Lexer(std::string_view file, std::string_view source);
	bool failed() const;

	std::vector<Token> tokenize();

private:
	std::string_view file, source;
	bool error;

	Token tokenize_string_literal(std::string_view::const_iterator &it, std::string_view::const_iterator end, unsigned &line, unsigned &column);
	Token tokenize_operator_separator(std::string_view::const_iterator &it, std::string_view::const_iterator end, unsigned &line, unsigned &column);
	Token tokenize_number_literal(std::string_view::const_iterator &it, std::string_view::const_iterator end, unsigned &line, unsigned &column);
	Token tokenize_identifier_keyword(std::string_view::const_iterator &it, std::string_view::const_iterator end, unsigned &line, unsigned &column);
};
