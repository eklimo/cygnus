#include "parser.h"

#include "lang.h"

Parser::Parser(std::vector<Token> &tokens)
	: it(tokens.cbegin()), end(tokens.cend())
{}

using ParseError = Util::CompilerError;

// parser

std::unique_ptr<Node> Parser::parse()
{
	std::unique_ptr<Node> root = expression();

	if(it < end)
	{
		throw ParseError("Unexpected symbol '", it->value, "'");
	}

	return root;
}

// expression parsing

constexpr int infix_precedence(const Token &token)
{
	switch(token.type)
	{
		case TokenType::Operator:
		{
			std::string_view sym = token.value;
			if(sym == "*" || sym == "/")
			{
				return 20;
			}
			else if(sym == "+" || sym == "-")
			{
				return 10;
			}
		}
		default:
			return -1;
	}
}

constexpr int prefix_precedence(const Token &token)
{
	std::string_view sym = token.value;
	if(sym == "+" || sym == "-")
	{
		return 100;
	}
	else if(sym == "(")
	{
		return 0;
	}
	return -1;
}

std::unique_ptr<Node> Parser::null_denotation(const Token &token)
{
	int prec = prefix_precedence(token);

	switch(token.type)
	{
		case TokenType::Number:
			return std::make_unique<NumberLiteral>(token.value);

		case TokenType::String:
			return std::make_unique<StringLiteral>(token.value);

		case TokenType::Identifier:
			return std::make_unique<Identifier>(token.value);

		case TokenType::Keyword:
		{
			if(Lang::is_boolean(token.value))
			{
				return std::make_unique<BooleanLiteral>(token.value);
			}
			else
			{
				throw ParseError("Unexpected keyword '", token.value, "'");
			}
		}

		case TokenType::Operator:
		{
			if(prec == -1)
			{
				throw ParseError("Expected operand before '", token.value, "'");
			}
			auto operand = expression(prec);
			if(!operand)
			{
				throw ParseError("Expected operand after '", token.value, "'");
			}
			return std::make_unique<PrefixOperator>(token.value, std::move(operand));
		}
		case TokenType::Separator:
		{
			if(token.value == "(")
			{
				auto expr = expression(prec);
				if(!match(")"))
				{
					throw ParseError("Expected ')'");
				}
				return expr;
			}
			else
			{
				throw ParseError("Unexpected symbol '", token.value, "'");
			}
		}

		default:
			return nullptr;
	}
}

std::unique_ptr<Node> Parser::left_denotation(const Token &token, std::unique_ptr<Node> &left)
{
	int prec = infix_precedence(token);
	if(prec == -1)
	{
		throw ParseError("Unexpected symbol '", token.value, "'");
	}
	switch(token.type)
	{
		case TokenType::Operator:
		{
			auto right = expression(prec);
			if(!right)
			{
				throw ParseError("Expected operand after '", token.value, "'");
			}

			return std::make_unique<InfixOperator>(token.value, std::move(left), std::move(right));
		}
		default:
			return nullptr;
	}
}

std::unique_ptr<Node> Parser::expression(int rbp)
{
	auto const &first = token();
	advance();
	auto tree = null_denotation(first);
	while(it <= end && rbp < infix_precedence(token()))
	{
		auto const &next = token();
		advance();
		tree = left_denotation(next, tree);
	}
	return tree;
}

// utility

const Token &Parser::token() const
{
	return *it;
}

bool Parser::advance()
{
	it++;
	return it < end;
}

bool Parser::match(TokenType type)
{
	if(it == end)
	{
		return false;
	}

	if(it->type == type)
	{
		advance();
		return true;
	}
	return false;
}

bool Parser::match(std::string_view value)
{
	if(it == end)
	{
		return false;
	}

	if(it->value == value)
	{
		advance();
		return true;
	}
	return false;
}

bool Parser::match(TokenType type, std::string_view value)
{
	return it->value == value && match(type);
}
