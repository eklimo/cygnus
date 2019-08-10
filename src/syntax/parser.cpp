#include "parser.h"

#include "lang.h"

Parser::Parser(std::vector<Token> &tokens)
	: it(tokens.cbegin()),
	  begin(tokens.cbegin()),
	  end(tokens.cend()),
	  stringifier()
{}

using ParseError = Util::CompilerError;

std::unique_ptr<Node> Parser::parse()
{
	std::unique_ptr<Node> root = expression();

	if(it < end)
		throw ParseError("Unexpected symbol '", it->value, "'");

	return root;
}

// expressions

std::unique_ptr<Node> Parser::null_denotation(const Token &tok)
{
	int prec = Lang::null_precedence(tok);

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
				return std::make_unique<BooleanLiteral>(tok.value);
			else
				throw ParseError("Unexpected keyword '", tok.value, "'");
		}

		case TokenType::Operator:
		{
			auto operand = expression(prec);
			if(!operand)
			{
				if((Lang::null_precedence(tok) != -1 && Lang::left_precedence(tok) != -1) && token().type != TokenType::Invalid)
					throw ParseError("Unexpected symbol '", token().value, "'");
				else if(token().type == TokenType::Invalid)
					throw ParseError("Expected operand after '", (it - 1)->value, "'");
				else
					throw ParseError("Unexpected symbol '", token().value, "'");
			}

			return std::make_unique<PrefixOperator>(tok.value, std::move(operand));
		}

		case TokenType::Separator:
		{
			if(tok.value == "(")
			{
				auto expr = expression(prec);
				if(!expr)
				{
					if(token().type == TokenType::Invalid || token().value == ")")
						throw ParseError("Expected expression after '", (it - 1)->value, "'");
					else
						throw ParseError("Unexpected symbol '", token().value, "'");
				}

				if(!match(")"))
				{
					if(token().type == TokenType::Invalid)
						throw ParseError("Expected ')' after '", (it - 1)->value, "'");
					else
						throw ParseError("Unexpected symbol '", token().value, "'");
				}

				return expr;
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

std::unique_ptr<Node> Parser::left_denotation(const Token &tok, std::unique_ptr<Node> &left)
{
	int prec = Lang::left_precedence(tok);
	if(prec == -1)
		throw ParseError("Unexpected symbol '", tok.value, "'");

	switch(tok.type)
	{
		case TokenType::Operator:
		{
			// postfix
			if(Lang::is_postfix(tok))
			{
				return std::make_unique<PostfixOperator>(tok.value, std::move(left));
			}
			// infix
			else
			{
				auto right = expression(Lang::is_right_associative(tok) ? prec - 1 : prec);

				if(!right)
				{
					if(token().type == TokenType::Invalid)
						throw ParseError("Expected operand after '", (it - 1)->value, "'");
					else
						throw ParseError("Unexpected symbol '", token().value, "'");
				}

				return std::make_unique<InfixOperator>(tok.value, std::move(left), std::move(right));
			}
		}

		case TokenType::Separator:
		{
			if(tok.value == "(")
			{
				// prevent calls on invalid tokens
				if(is_valid_index(-2))
				{
					if((it - 2)->type == TokenType::Operator)
					{
						throw ParseError("Unexpected token '", tok.value, "'");
					}
				}

				std::vector<std::unique_ptr<Node>> arguments;

				if(token().value != ")")
				{
					while(true)
					{
						if(token().value == ")")
						{
							if((it - 1)->value == ",")
								throw ParseError("Expected expression after ','");
							else
								throw ParseError("Expected expression before ')'");
						}

						auto arg = expression(0);
						if(!arg)
						{
							if(token().type == TokenType::Invalid)
								throw ParseError("Expected expression after '", (it - 1)->value, "'");
							else
								throw ParseError("Unexpected token '", token().value, "'");
						}
						arguments.push_back(std::move(arg));

						if(token().value != ",")
							break;

						match(",");
					}
				}

				if(!match(")"))
				{
					if(token().type == TokenType::Invalid)
						throw ParseError("Expected ')' after '", (it - 1)->value, "'");
					else
						throw ParseError("Unexpected symbol '", token().value, "'");
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
	if(Lang::null_precedence(token()) == -1)
		return nullptr;

	auto const &first = token();
	advance();
	auto tree = null_denotation(first);
	while(it < end && rbp < Lang::left_precedence(token()))
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
	static Token inv =
	{
		.type = TokenType::Invalid,
		.value = "",
		.line = 0,
		.column = 0
	};
	if(it == end) return inv;

	return *it;
}

bool Parser::advance()
{
	it++;
	return it < end;
}

bool Parser::is_valid_index(int n) const
{
	return it + n >= begin && it + n <= end;
}

bool Parser::match(TokenType type)
{
	if(it == end)
		return false;

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
		return false;

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
