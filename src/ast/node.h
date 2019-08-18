// generated by nodegen.py; do not edit

#pragma once

#include <string_view>
#include <memory>
#include <vector>

#include "visitor.h"

struct Node
{
	virtual void accept(Visitor &v) = 0;
};
struct Expression : public Node
{
};
struct Statement : public Node
{
};
struct Program : public Node
{
	std::vector<std::unique_ptr<Statement>> statements;
	Program(std::vector<std::unique_ptr<Statement>> statements);
	void accept(Visitor &v) override;
};
struct ExprStatement : public Statement
{
	std::unique_ptr<Expression> expr;
	ExprStatement(std::unique_ptr<Expression> expr);
	void accept(Visitor &v) override;
};
struct VariableDef : public Statement
{
	std::unique_ptr<Identifier> name;
	std::unique_ptr<Type> type;
	std::unique_ptr<Expression> value;
	VariableDef(std::unique_ptr<Identifier> name, std::unique_ptr<Type> type, std::unique_ptr<Expression> value);
	void accept(Visitor &v) override;
};
struct FunctionDef : public Statement
{
	std::unique_ptr<Identifier> name;
	std::vector<std::unique_ptr<Parameter>> parameters;
	std::unique_ptr<Type> return_type;
	std::unique_ptr<Block> body;
	FunctionDef(std::unique_ptr<Identifier> name, std::vector<std::unique_ptr<Parameter>> parameters, std::unique_ptr<Type> return_type, std::unique_ptr<Block> body);
	void accept(Visitor &v) override;
};
struct Value : public Expression
{
	std::string_view value;
	Value(std::string_view value);
};
struct Literal : public Value
{
	using Value::Value;
};
struct NumberLiteral : public Literal
{
	using Literal::Literal;
	void accept(Visitor &v) override;
};
struct StringLiteral : public Literal
{
	using Literal::Literal;
	void accept(Visitor &v) override;
};
struct BooleanLiteral : public Literal
{
	using Literal::Literal;
	void accept(Visitor &v) override;
};
struct Identifier : public Value
{
	using Value::Value;
	void accept(Visitor &v) override;
};
struct FunctionCall : public Expression
{
	std::unique_ptr<Identifier> name;
	std::vector<std::unique_ptr<Expression>> arguments;
	FunctionCall(std::unique_ptr<Identifier> name, std::vector<std::unique_ptr<Expression>> arguments);
	void accept(Visitor &v) override;
};
struct Operator : public Expression
{
	std::string_view symbol;
	Operator(std::string_view symbol);
};
struct InfixOperator : public Operator
{
	std::unique_ptr<Expression> left;
	std::unique_ptr<Expression> right;
	InfixOperator(std::string_view symbol, std::unique_ptr<Expression> left, std::unique_ptr<Expression> right);
	void accept(Visitor &v) override;
};
struct PrefixOperator : public Operator
{
	std::unique_ptr<Expression> operand;
	PrefixOperator(std::string_view symbol, std::unique_ptr<Expression> operand);
	void accept(Visitor &v) override;
};
struct PostfixOperator : public Operator
{
	std::unique_ptr<Expression> operand;
	PostfixOperator(std::string_view symbol, std::unique_ptr<Expression> operand);
	void accept(Visitor &v) override;
};
struct ReturnExpr : public Expression
{
	std::unique_ptr<Expression> value;
	ReturnExpr(std::unique_ptr<Expression> value);
	void accept(Visitor &v) override;
};
struct Invalid : public Node
{
	void accept(Visitor &v) override;
};
struct Block : public Statement
{
	std::vector<std::unique_ptr<Statement>> statements;
	Block(std::vector<std::unique_ptr<Statement>> statements);
	void accept(Visitor &v) override;
};
struct Parameter : public Node
{
	std::unique_ptr<Identifier> name;
	std::unique_ptr<Type> type;
	Parameter(std::unique_ptr<Identifier> name, std::unique_ptr<Type> type);
	void accept(Visitor &v) override;
};
struct Type : public Node
{
	std::string_view value;
	Type(std::string_view value);
	void accept(Visitor &v) override;
};
