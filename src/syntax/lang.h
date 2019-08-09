#pragma once

#include <string_view>

namespace Lang
{
	constexpr std::string_view keywords[] = {"var", "true", "false"};
	// descending length
	constexpr std::string_view operators[] = {"==", "++", "--", "=", "+", "-", "*", "/"};
	constexpr std::string_view word_operators[] = {"and", "or"};
	constexpr std::string_view separators[] = {"(", ")", "[", "]", "{", "}", ",", "."};

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

	constexpr bool is_boolean(std::string_view str)
	{
		return str == "true" || str == "false";
	}
}
