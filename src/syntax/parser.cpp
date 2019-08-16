#include "parser.h"

#include <type_traits>

#include "lang.h"

using Util::Error;

Parser::Parser(const std::vector<Token> &tokens)
	: it(tokens.cbegin()),
	  begin(tokens.cbegin()),
	  end(tokens.cend())
{}

template<typename Base, typename Derived>
std::unique_ptr<Derived> cast(typename std::remove_reference<std::unique_ptr<Base> &>::type p)
{
	return std::unique_ptr<Derived>(static_cast<Derived *>(p.release()));
}

// main

std::unique_ptr<Program> Parser::parse()
{
	auto root = program();

	if(it < end)
		throw Error::At(it, "unexpected symbol '", it->value, "'");

	return root;
}

std::unique_ptr<Program> Parser::program()
{
	std::vector<std::unique_ptr<Statement>> statements;

	std::unique_ptr<Statement> stmt;
	while((stmt = statement()))
	{
		statements.push_back(std::move(stmt));
	}

	return std::make_unique<Program>(std::move(statements));
}

// statements

std::unique_ptr<Statement> Parser::statement()
{
	std::unique_ptr<Statement> stmt;
	if
	(
	    (stmt = variable_def()) ||
	    (stmt = expr_statement())
	)
		return stmt;

	return nullptr;
}

std::unique_ptr<Statement> Parser::expr_statement()
{
	auto expr = expression();
	if(expr)
	{
		return std::make_unique<ExprStatement>(std::move(expr));
	}

	return nullptr;
}

std::unique_ptr<VariableDef> Parser::variable_def()
{
	if(match(TokenType::Keyword, "var"))
	{
		auto name_token = token();
		if(match(TokenType::Identifier))
		{
			auto name = std::make_unique<Identifier>(name_token.value);
			if(match("="))
			{
				auto value = expression();
				if(value)
				{
					return std::make_unique<VariableDef>(std::move(name), std::move(value));
				}
				else
				{
					if(token().type == TokenType::Invalid)
						throw Error::After(it - 1, "expected expression after '", last_token().value, "'");
					else
						throw Error::At(it, "unexpected symbol '", token().value, "'");
				}
			}
			else throw Error::After(it - 1, "expected '=' after '", name_token.value, "'");
		}
		else throw Error::After(it - 1, "expected identifier after 'var'");
	}
	return nullptr;
}

// expressions

std::unique_ptr<Expression> Parser::expression(int rbp)
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
		tree = left_denotation(next, std::move(tree));
	}
	return tree;
}

std::unique_ptr<Expression> Parser::null_denotation(const Token &tok)
{
	switch(tok.type)
	{
		case TokenType::Number:
		case TokenType::String:
		case TokenType::Keyword:
			return literal(tok);

		case TokenType::Identifier:
			return std::make_unique<Identifier>(tok.value);

		case TokenType::Operator:
			return prefix_operator_expr(tok);

		case TokenType::Separator:
		{
			if(tok.value == "(")
				return group_expr(tok);
			else
				return nullptr;
		}

		default:
			return nullptr;
	}
}

std::unique_ptr<Expression> Parser::left_denotation(const Token &tok, std::unique_ptr<Expression> left)
{
	switch(tok.type)
	{
		case TokenType::Operator:
		{
			if(Lang::is_postfix(tok))
				return postfix_operator_expr(tok, std::move(left));
			else
				return infix_operator_expr(tok, std::move(left));
		}

		case TokenType::Separator:
		{
			if(tok.value == "(")
				return call_expr(tok, std::move(left));
			else
				return nullptr;
		}

		default:
			return nullptr;
	}
}


std::unique_ptr<Expression> Parser::prefix_operator_expr(const Token &tok)
{
	int prec = Lang::null_precedence(tok);

	auto operand = expression(prec);
	if(!operand)
	{
		if((Lang::null_precedence(tok) != -1 && Lang::left_precedence(tok) != -1) && token().type != TokenType::Invalid)
			throw Error::At(it, "unexpected symbol '", token().value, "'");
		else if(token().type == TokenType::Invalid)
			throw Error::After(it - 1, "expected operand after '", last_token().value, "'");
		else
			throw Error::At(it, "unexpected symbol '", token().value, "'");
	}

	return std::make_unique<PrefixOperator>(tok.value, std::move(operand));
}

std::unique_ptr<Expression> Parser::infix_operator_expr(const Token &tok, std::unique_ptr<Expression> left)
{
	int prec = Lang::left_precedence(tok);

	auto right = expression(Lang::is_right_associative(tok) ? prec - 1 : prec);
	if(!right)
	{
		if(token().type == TokenType::Invalid)
			throw Error::After(it - 1, "expected operand after '", last_token().value, "'");
		else
			throw Error::At(it, "unexpected symbol '", token().value, "'");
	}

	return std::make_unique<InfixOperator>(tok.value, std::move(left), std::move(right));
}

std::unique_ptr<Expression> Parser::postfix_operator_expr(const Token &tok, std::unique_ptr<Expression> left)
{
	return std::make_unique<PostfixOperator>(tok.value, std::move(left));
}

std::unique_ptr<Expression> Parser::group_expr(const Token &tok)
{
	int prec = Lang::null_precedence(tok);

	auto expr = expression(prec);
	if(!expr)
	{
		if(token().type == TokenType::Invalid || token().value == ")")
			throw Error::After(it - 1, "expected expression after '", last_token().value, "'");
		else
			throw Error::At(it, "unexpected symbol '", token().value, "'");
	}

	if(!match(")"))
	{
		if(token().type == TokenType::Invalid)
			throw Error::After(it - 1, "expected ')' after '", last_token().value, "'");
		else
			throw Error::At(it, "unexpected symbol '", token().value, "'");
	}

	return expr;
}

std::unique_ptr<Expression> Parser::call_expr(const Token &tok, std::unique_ptr<Expression> left)
{
	// prevent calls on invalid tokens
	if(is_valid_index(-2))
	{
		if((it - 2)->type != TokenType::Identifier)
		{
			throw Error::At(it - 1, "unexpected symbol '", last_token().value, "'");
		}
	}

	std::vector<std::unique_ptr<Expression>> arguments;

	if(token().value != ")")
	{
		while(true)
		{
			auto arg = expression(0);
			if(!arg)
			{
				if(token().type == TokenType::Invalid)
					throw Error::After(it - 1, "expected expression after '", last_token().value, "'");
				else
					throw Error::At(it, "unexpected symbol '", token().value, "'");
			}
			arguments.push_back(std::move(arg));

			if(token().value != ",")
				break;

			match(",");

			if(token().value == ")")
			{
				throw Error::At(it, "unexpected symbol '", it->value, "'");
			}
		}
	}

	if(!match(")"))
	{
		if(token().type == TokenType::Invalid)
			throw Error::After(it - 1, "expected ')' after '", last_token().value, "'");
		else
			throw Error::At(it, "unexpected symbol '", token().value, "'");
	}

	return std::make_unique<FunctionCall>(cast<Expression, Identifier>(std::move(left)), std::move(arguments));
}

std::unique_ptr<Expression> Parser::literal(const Token &tok)
{
	switch(tok.type)
	{
		case TokenType::Number:
			return std::make_unique<NumberLiteral>(tok.value);

		case TokenType::String:
			return std::make_unique<StringLiteral>(tok.value);

		case TokenType::Keyword:
		{
			if(Lang::is_boolean(tok.value))
				return std::make_unique<BooleanLiteral>(tok.value);
			else
				throw Error::At(it - 1, "unexpected symbol '", last_token().value, "'");
		}

		default:
			return nullptr;
	}
}

// general

// utility

const Token &Parser::token() const
{
	static Token inv;
	if(it == end) return inv;

	return *it;
}

const Token &Parser::last_token() const
{
	return *(it - 1);
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
