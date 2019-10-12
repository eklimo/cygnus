#pragma once

#include "syntax/token.h"

#include <string_view>

namespace Lang
{
	constexpr std::string_view keywords[] = {"true", "false", "var", "func", "return", "if", "else", "while"};
	// descending length
	constexpr std::string_view operators[] = {"++", "--", "==", "!=", ">=", "<=", "&&", "||", "!", ">", "<", "+", "-", "*", "/", "%", "="};
	constexpr std::string_view word_operators[] = {"not", "and", "or"};
	// descending length
	constexpr std::string_view separators[] = {"->", "(", ")", ",", "{", "}", ":", ";"};

	// lexer

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

	// parser

	constexpr int null_precedence(const Token &token)
	{
		auto sym = token.value;

		switch(token.type)
		{
			case TokenType::Operator:
			{
				if(sym == "++" || sym == "--" || sym == "+" || sym == "-" || sym == "!" || sym == "not")
					return 14;
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
			case TokenType::Keyword:
			{
				if(is_boolean(sym) || sym == "return" || sym == "if" || sym == "while")
					return 0;
				break;
			}
			default:
				return -1;
		}

		return -1;
	}

	constexpr int left_precedence(const Token &token)
	{
		auto sym = token.value;

		switch(token.type)
		{
			case TokenType::Operator:
			{
				if(sym == "++" || sym == "--")
					return 15;
				else if(sym == "*" || sym == "/" || sym == "%")
					return 12;
				else if(sym == "+" || sym == "-")
					return 11;
				else if(sym == ">=" || sym == "<=" || sym == ">" || sym == "<" || sym == "==" || sym == "!=")
					return 9;
				else if(sym == "&&" || sym == "and")
					return 4;
				else if(sym == "||" || sym == "or")
					return 3;
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

		auto sym = token.value;

		if(sym == "++" || sym == "--")
			return true;

		return false;
	}

	constexpr bool is_right_associative(const Token &token)
	{
		if(token.type != TokenType::Operator) return false;

		auto sym = token.value;

		if(sym == "=")
			return true;

		return false;
	}

	// operator class

	constexpr bool is_assignment(std::string_view op)
	{
		return op == "=";
	}

	constexpr bool is_arithmetic(std::string_view op)
	{
		return op == "+" || op == "-" || op == "*" || op == "/" || op == "%" || op == "++" || op == "--";
	}

	constexpr bool is_boolean_op(std::string_view op)
	{
		return op == "==" || op == "!=" || op == ">" || op == ">=" || op == "<" || op == "<=" || op == "!" || op == "not" || op == "&&" || op == "and" || op == "||" || op == "or";
	}
}
