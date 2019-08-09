#include "parser.h"

#include "lang.h"

Parser::Parser(std::vector<Token> &tokens)
	: it(tokens.cbegin()), end(tokens.cend()), stringifier()
{}

using ParseError = Util::CompilerError;

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
	std::string_view sym = token.value;

	switch(token.type)
	{
		case TokenType::Operator:
		{
			if(sym == "*" || sym == "/")
			{
				return 20;
			}
			else if(sym == "+" || sym == "-")
			{
				return 10;
			}
		}
		case TokenType::Separator:
		{
			if(sym == "(")
			{
				return 1000;
			}
		}
		default:
			return -1;
	}

	return -1;
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

std::unique_ptr<Node> Parser::null_denotation(const Token &tok)
{
	int prec = prefix_precedence(tok);
	switch(tok.type)
	{
		case TokenType::Number:
			return std::make_unique<NumberLiteral>(tok.value);

		case TokenType::String:
			return std::make_unique<StringLiteral>(tok.value);

		case TokenType::Identifier:
			return std::make_unique<Identifier>(tok.value);

		case TokenType::Keyword:
		{
			if(Lang::is_boolean(tok.value))
			{
				return std::make_unique<BooleanLiteral>(tok.value);
			}
			else
			{
				throw ParseError("Unexpected keyword '", tok.value, "'");
			}
		}

		case TokenType::Operator:
		{
			if(prec == -1)
			{
				throw ParseError("Expected operand before '", tok.value, "'");
			}
			auto operand = expression(prec);
			if(!operand)
			{
				throw ParseError("Expected operand after '", tok.value, "'");
			}
			return std::make_unique<PrefixOperator>(tok.value, std::move(operand));
		}
		case TokenType::Separator:
		{
			if(tok.value == "(")
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
				throw ParseError("Unexpected symbol '", tok.value, "'");
			}
		}

		default:
			return nullptr;
	}
}

std::unique_ptr<Node> Parser::left_denotation(const Token &tok, std::unique_ptr<Node> &left)
{
	int prec = infix_precedence(tok);
	if(prec == -1)
	{
		throw ParseError("Unexpected symbol '", tok.value, "'");
	}
	switch(tok.type)
	{
		case TokenType::Operator:
		{
			auto right = expression(prec);
			if(!right)
			{
				throw ParseError("Expected operand after '", tok.value, "'");
			}

			return std::make_unique<InfixOperator>(tok.value, std::move(left), std::move(right));
		}
		case TokenType::Separator:
		{
			if(tok.value == "(")
			{
				std::vector<std::unique_ptr<Node>> arguments;

				if(token().value != ")")
				{
					while(true)
					{
						if(token().value == ")")
						{
							if((it - 1)->value == ",")
							{
								throw ParseError("Expected expression after ','");
							}
							else
							{
								throw ParseError("Expected expression before ')'");
							}
						}
						else if(token().type == TokenType::Invalid)
						{
							throw ParseError("Expected expression after '", (it - 1)->value, "'");
						}

						auto arg = expression(0);
						arguments.push_back(std::move(arg));

						if(token().value != ",")
						{
							break;
						}
						match(",");
					}
				}

				if(!match(")"))
				{
					throw ParseError("Expected ')' after function call '", stringifier.stringify(*left.get()), "'");
				}

				return std::make_unique<FunctionCall>(std::move(left), std::move(arguments));
			}
			else
			{
				return nullptr;
			}
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
	while(it < end && rbp < infix_precedence(token()))
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
