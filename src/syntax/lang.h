#pragma once

#include <string_view>

#include "../syntax/token.h"

using Lexer::Token;
using Lexer::TokenType;

namespace Lang
{
	constexpr std::string_view keywords[] = {"true", "false", "var", "func", "return", "if", "else"};
	// descending length
	constexpr std::string_view operators[] = {"++", "--", "+", "-", "*", "/", "="};
	constexpr std::string_view word_operators[] = {};
	// descending length
	constexpr std::string_view separators[] = {"->", "(", ")", ",", "{", "}", ":"};

	// utility

	constexpr bool is_keyword(std::string_view str)
	{
		for(const auto &keyword : keywords)
		{
			if(str == keyword) return true;
		}
		return false;
	}

	constexpr bool is_operator(std::string_view str)
	{
		for(const auto &op : operators)
		{
			if(str == op) return true;
		}
		return false;
	}

	constexpr bool is_word_operator(std::string_view str)
	{
		for(const auto &op : word_operators)
		{
			if(str == op) return true;
		}
		return false;
	}

	constexpr bool is_separator(std::string_view str)
	{
		for(const auto &sep : separators)
		{
			if(str == sep) return true;
		}
		return false;
	}

	constexpr bool is_boolean(std::string_view str)
	{
		return str == "true" || str == "false";
	}

	// operator

	constexpr int null_precedence(const Token &token)
	{
		const std::string_view sym = token.value;

		switch(token.type)
		{
			case TokenType::Operator:
			{
				if(sym == "+" || sym == "-")
					return 30;
				else if(sym == "++" || sym == "--")
					return 30;
				break;
			}
			case TokenType::Separator:
			{
				if(sym == "(")
					return 0;
				break;
			}
			case TokenType::Number:
			case TokenType::String:
			case TokenType::Identifier:
				return 0;
			default:
				return -1;
		}

		return -1;
	}

	constexpr int left_precedence(const Token &token)
	{
		const std::string_view sym = token.value;

		switch(token.type)
		{
			case TokenType::Operator:
			{
				if(sym == "++" || sym == "--")
					return 40;
				else if(sym == "*" || sym == "/")
					return 20;
				else if(sym == "+" || sym == "-")
					return 10;
				else if(sym == "=")
					return 1;
				break;
			}
			case TokenType::Separator:
			{
				if(sym == "(")
					return 1000;
				break;
			}
			default:
				return -1;
		}

		return -1;
	}

	constexpr bool is_postfix(const Token &token)
	{
		if(token.type != TokenType::Operator) return false;

		const std::string_view sym = token.value;

		if(sym == "++" || sym == "--")
			return true;

		return false;
	}

	constexpr bool is_right_associative(const Token &token)
	{
		if(token.type != TokenType::Operator) return false;

		const std::string_view sym = token.value;

		if(sym == "=")
			return true;

		return false;
	}
}
