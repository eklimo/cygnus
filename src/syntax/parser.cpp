#include "parser.h"

#include <type_traits>

#include "lang.h"

using Util::Error;

Parser::Parser(const std::vector<Token> &tokens, std::string_view file, std::string_view source)
	: it(tokens.cbegin()),
	  begin(tokens.cbegin()),
	  end(tokens.cend()),
	  file(file),
	  source(source),
	  error(false)
{
}

bool Parser::failed() const
{
	return error;
}

template<typename Base, typename Derived>
std::unique_ptr<Derived> cast(typename std::remove_reference<std::unique_ptr<Base> &>::type p)
{
	return std::unique_ptr<Derived>(static_cast<Derived *>(p.release()));
}

// main

std::unique_ptr<Program> Parser::parse()
{
	auto root = program();

	return root;
}

std::unique_ptr<Program> Parser::program()
{
	auto statements = statement_list();

	if(it < end)
	{
		error = true;
		Error::At(*it, "unexpected symbol '", it->value, "'").print(file, source);

		// find more errors
		advance();
		program();
	}

	return std::make_unique<Program>(std::move(statements));
}

// statements

std::unique_ptr<Statement> Parser::statement()
{
	std::unique_ptr<Statement> stmt;
	try
	{
		if
		(
		    (stmt = variable_def()) ||
		    (stmt = function_def()) ||
		    (stmt = expr_statement()) ||
		    (stmt = block())
		) return stmt;
	}
	catch(Util::Error &e)
	{
		error = true;
		e.print(file, source);
		return panic();
	}

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
	if(match("var"))
	{
		auto name_token = match(TokenType::Identifier);
		if(!name_token)
			expect("name");
		auto name = std::make_unique<Identifier>(*name_token, nullptr);

		auto typ = type_annotation();

		std::unique_ptr<Expression> value;
		if(match("="))
		{
			value = expression();
			if(!value)
				expect("expression");
		}

		if(!value && !typ)
			expect("'=' or type annotation");

		return std::make_unique<VariableDef>(std::move(name), std::move(typ), std::move(value));
	}

	return nullptr;
}

std::unique_ptr<FunctionDef> Parser::function_def()
{
	if(match("func"))
	{
		auto name_token = match(TokenType::Identifier);
		if(!name_token)
			expect("name");
		auto name = std::make_unique<Identifier>(*name_token, nullptr);

		if(!match("("))
			expect("'('");

		std::vector<std::unique_ptr<Parameter>> parameters;

		while(true)
		{
			if(token().value == ")" && last_token().value != ",") break;

			auto param = parameter();
			if(!param)
				expect(last_token().value == "," ? "parameter" : "parameter or ')'");
			parameters.push_back(std::move(param));

			if(token().value != ")" && token().value != ",")
				expect("')' or ','");

			match(",");
		}

		if(!match(")"))
			expect("')'");

		std::unique_ptr<Type> typ;
		if(match("->"))
		{
			typ = type();
			if(!typ)
				expect("type");
		}

		auto body = block();
		if(!body)
			expect("'{'");

		return std::make_unique<FunctionDef>(std::move(name), std::move(parameters), std::move(typ), std::move(body));
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
	trim();

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
			return literal(tok);

		case TokenType::Keyword:
		{
			if(Lang::is_boolean(tok.value))
				return literal(tok);
			else if(tok.value == "return")
				return return_expr(tok);
			else if(tok.value == "if")
				return if_expr(tok);
			else if(tok.value == "while")
				return while_expr(tok);

			return nullptr;
		}

		case TokenType::Identifier:
			return std::make_unique<Identifier>(tok, nullptr);

		case TokenType::Operator:
			return prefix_operator_expr(tok);

		case TokenType::Separator:
		{
			if(tok.value == "(")
			{
				auto expr = group_expr(tok);

				// unit
				if(!expr)
					expr = literal(tok);

				if(!expr)
					expect("expression or ')'");

				return std::move(expr);
			}
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
		expect("expression");

	return std::make_unique<PrefixOperator>(tok, std::move(operand));
}

std::unique_ptr<Expression> Parser::infix_operator_expr(const Token &tok, std::unique_ptr<Expression> left)
{
	int prec = Lang::left_precedence(tok);

	auto right = expression(Lang::is_right_associative(tok) ? prec - 1 : prec);
	if(!right)
		expect("expression");

	return std::make_unique<InfixOperator>(tok, std::move(left), std::move(right));
}

std::unique_ptr<Expression> Parser::postfix_operator_expr(const Token &tok, std::unique_ptr<Expression> left)
{
	return std::make_unique<PostfixOperator>(tok, std::move(left));
}

std::unique_ptr<Expression> Parser::group_expr(const Token &tok)
{
	int prec = Lang::null_precedence(tok);

	auto expr = expression(prec);
	if(expr)
	{
		if(!match(")"))
			expect("')'");

		return expr;
	}

	return nullptr;
}

std::unique_ptr<Expression> Parser::call_expr(const Token &tok, std::unique_ptr<Expression> left)
{
	// prevent calls on invalid tokens
	if(is_valid_index(-2))
	{
		if((it - 2)->type != TokenType::Identifier)
		{
			error = true;
			Error::At(*(it - 1), "unexpected symbol '", last_token().value, "'").print(file, source);
		}
	}

	std::vector<std::unique_ptr<Expression>> arguments;

	while(true)
	{
		if(token().value == ")" && last_token().value != ",") break;

		auto arg = expression(0);
		if(!arg)
			expect(last_token().value == "," ? "expression" : "expression or ')'");
		arguments.push_back(std::move(arg));

		if(token().value != ")" && token().value != ",")
			expect("')' or ','");

		match(",");
	}

	if(!match(")"))
		expect("')'");

	return std::make_unique<FunctionCall>(cast<Expression, Identifier>(std::move(left)), std::move(arguments));
}

std::unique_ptr<Expression> Parser::return_expr(const Token &tok)
{
	auto value = expression();

	return std::make_unique<ReturnExpr>(std::move(value));
}

std::unique_ptr<Expression> Parser::if_expr(const Token &tok)
{
	auto condition = expression();
	if(!condition)
		expect("expression");

	auto if_branch = block();
	if(!if_branch)
		expect("'{'");

	std::unique_ptr<Block> else_branch;
	if(match("else"))
	{
		else_branch = block();
		if(!else_branch)
			expect("'{'");
	}

	return std::make_unique<IfExpr>(std::move(condition), std::move(if_branch), std::move(else_branch));
}

std::unique_ptr<Expression> Parser::while_expr(const Token &tok)
{
	auto condition = expression();
	if(!condition)
		expect("expression");

	auto body = block();
	if(!body)
		expect("'{'");

	return std::make_unique<WhileExpr>(std::move(condition), std::move(body));
}

std::unique_ptr<Expression> Parser::literal(const Token &tok)
{
	switch(tok.type)
	{
		case TokenType::Number:
			return std::make_unique<NumberLiteral>(tok);

		case TokenType::String:
			return std::make_unique<StringLiteral>(tok);

		case TokenType::Keyword:
		{
			if(Lang::is_boolean(tok.value))
				return std::make_unique<BooleanLiteral>(tok);
			else
				return nullptr;
		}

		case TokenType::Separator:
		{
			// unit
			if(tok.value == "(")
			{
				if(match(")"))
				{
					return std::make_unique<UnitLiteral>(Token
					{
						.type = TokenType::Separator,
						.value = "()",
						.location = tok.location
					});
				}

				return nullptr;
			}
			else
				return nullptr;
		}

		default:
			return nullptr;
	}
}

// general

std::vector<std::unique_ptr<Statement>> Parser::statement_list()
{
	std::vector<std::unique_ptr<Statement>> statements;

	std::unique_ptr<Statement> stmt = statement();
	while(stmt)
	{
		statements.push_back(std::move(stmt));

		// semicolon separation
		if(it->value == ";")
		{
			// reject semicolon at beginning and end of line
			if((it - 1)->value != "\n" && (it + 1)->value != "\n")
				match(";");
			else
				break;

			stmt = statement();
		}
		// newline separation
		else
		{
			auto sep = (it - 1);
			stmt = statement();
			if(stmt && sep->value != "\n")
			{
				it = sep + 1;
				expect("newline or ';'", false).print(file, source);
				advance();
				stmt = panic();
			}
		}
	}

	return std::move(statements);
}

std::unique_ptr<Block> Parser::block()
{
	if(match("{"))
	{
		auto statements = statement_list();

		if(!match("}"))
			expect("'}'");

		return std::make_unique<Block>(std::move(statements));
	}

	return nullptr;
}

std::unique_ptr<Parameter> Parser::parameter()
{
	auto name_token = match(TokenType::Identifier);
	if(name_token)
	{
		auto name = std::make_unique<Identifier>(*name_token, nullptr);

		auto type = type_annotation();
		if(!type)
			expect("type annotation");

		return std::make_unique<Parameter>(std::move(name), std::move(type));
	}

	return nullptr;
}

std::unique_ptr<Type> Parser::type_annotation()
{
	if(match(":"))
	{
		auto typ = type();
		if(!typ)
			expect("type");

		return typ;
	}

	return nullptr;
}

std::unique_ptr<Type> Parser::type()
{
	// name
	auto name_token = match(TokenType::Identifier);
	if(name_token)
	{
		return std::make_unique<Type>(*name_token);
	}
	// unit
	else
	{
		auto lparen = match("(");
		if(lparen)
		{
			if(!match(")"))
			{
				error = true;
				Error::At(*(it - 1), "unexpected symbol '('");
			}

			return std::make_unique<Type>(Token
			{
				.type = TokenType::Separator,
				.value = "()",
				.location = lparen->location
			});
		}
	}
	return nullptr;
}

// utility

const Token &Parser::token() const
{
	static Token inv;
	if(it == end) return inv;

	return *it;
}

const Token &Parser::last_token() const
{
	unsigned offset = 1;
	while((it - offset)->type == TokenType::Separator && (it - offset)->value == "\n")
		offset++;
	return *(it - offset);
}

void Parser::advance()
{
	it++;
}

bool Parser::is_valid_index(int n) const
{
	return it + n >= begin && it + n <= end;
}

std::optional<const Token> Parser::match(TokenType type)
{
	if(it == end)
		return {};

	trim();
	if(it->type == type)
	{
		auto old = it;
		advance();
		trim();
		return *old;
	}

	return {};
}

std::optional<const Token> Parser::match(std::string_view value)
{
	if(it == end)
		return {};

	trim();
	if(it->value == value)
	{
		auto old = it;
		advance();
		trim();
		return *old;
	}

	return {};
}

std::optional<const Token> Parser::match(TokenType type, std::string_view value)
{
	trim();
	if(it->value == value) return match(type);
	else return {};
}

void Parser::trim()
{
	while(it->type == TokenType::Separator && it->value == "\n")
		advance();
}

Util::Error Parser::expect(std::string_view expect, bool _throw)
{
	error = true;

	trim();

	std::string value = "'" + std::string(it->value) + "'";
	switch(it->type)
	{
		case TokenType::Keyword:
			value = "keyword " + value;
			break;
		case TokenType::String:
			value = "string " + value;
			break;
		default:
			break;
	}

	auto error =
	    it->type == TokenType::Invalid
	    ? Error::After(*(it - 2), "expected ", expect)
	    : Error::At(*it, "expected ", expect, ", found ", value);

	if(_throw)
		throw error;
	else
		return error;
}

std::unique_ptr<Statement> Parser::panic()
{
	std::unique_ptr<Statement> stmt;

	while(!(it->type == TokenType::Separator && (it->value == "\n" || it->value == ";")) && it < end)
		advance();
	if(it->value == ";")
		advance();

	stmt = statement();
	return stmt;
}
