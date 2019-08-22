#pragma once

#include <vector>
#include <memory>
#include <string_view>
#include <exception>
#include <sstream>
#include <optional>

#include "../ast/node.h"
#include "../util/error.h"
#include "../util/stringify_visitor.h"
#include "lexer.h"
#include "token.h"

using Lexer::Token;
using Lexer::TokenType;

class Parser
{
public:
	Parser(const std::vector<Token> &tokens, std::string_view file, std::string_view source);
	std::unique_ptr<Program> parse();

private:
	std::vector<Token>::const_iterator it;
	const std::vector<Token>::const_iterator begin, end;
	std::string_view file, source;
	bool error;

	std::unique_ptr<Program> program();
	std::unique_ptr<Statement> statement();
	std::unique_ptr<Statement> expr_statement();

	// statements
	std::unique_ptr<VariableDef> variable_def();
	std::unique_ptr<FunctionDef> function_def();

	// expressions
	std::unique_ptr<Expression> expression(int rbp = 0);
	std::unique_ptr<Expression> null_denotation(const Token &token);
	std::unique_ptr<Expression> left_denotation(const Token &token, std::unique_ptr<Expression> left);

	std::unique_ptr<Expression> prefix_operator_expr(const Token &token);
	std::unique_ptr<Expression> infix_operator_expr(const Token &token, std::unique_ptr<Expression> left);
	std::unique_ptr<Expression> postfix_operator_expr(const Token &token, std::unique_ptr<Expression> left);
	std::unique_ptr<Expression> group_expr(const Token &token);
	std::unique_ptr<Expression> call_expr(const Token &token, std::unique_ptr<Expression> left);
	std::unique_ptr<Expression> return_expr(const Token &token);
	std::unique_ptr<Expression> if_expr(const Token &token);
	std::unique_ptr<Expression> while_expr(const Token &token);
	std::unique_ptr<Expression> literal(const Token &token);

	// general
	std::vector<std::unique_ptr<Statement>> statement_list();
	std::unique_ptr<Block> block();
	std::unique_ptr<Parameter> parameter();
	std::unique_ptr<Type> type_annotation();
	std::unique_ptr<Type> type();

	// utility
	const Token &token() const;
	const Token &last_token() const;
	void advance();
	bool is_valid_index(int n) const;
	std::optional<const Token> match(TokenType type);
	std::optional<const Token> match(std::string_view value);
	std::optional<const Token> match(TokenType type, std::string_view value);
	void trim();
	Util::Error expect(std::string_view expect, bool _throw = true);
	std::unique_ptr<Statement> panic();
};
