#pragma once

#include <string_view>
#include <memory>

#include "visitor.h"

struct Node
{
	virtual void accept(Visitor &v) = 0;
};
struct Invalid: public Node
{
	virtual void accept(Visitor &v) override
	{
		v.visit(*this);
	}
};

struct Expression : public Node {};

struct Value : public Expression
{
	const std::string_view value;
	Value(std::string_view value);
};
struct Literal : public Value
{
	using Value::Value;
};
struct NumberLiteral : public Literal
{
	using Literal::Literal;
	virtual void accept(Visitor &v) override
	{
		v.visit(*this);
	}
};
struct StringLiteral : public Literal
{
	using Literal::Literal;
	virtual void accept(Visitor &v) override
	{
		v.visit(*this);
	}
};
struct BooleanLiteral : public Literal
{
	using Literal::Literal;
	virtual void accept(Visitor &v) override
	{
		v.visit(*this);
	}
};
struct Identifier : public Value
{
	using Value::Value;
	virtual void accept(Visitor &v) override
	{
		v.visit(*this);
	}
};

struct Operator : public Node
{
	const std::string_view symbol;
	Operator(std::string_view symbol);
};
struct InfixOperator : public Operator
{
	std::unique_ptr<Node> left;
	virtual void accept(Visitor &v) override
	{
		v.visit(*this);
	}
	std::unique_ptr<Node> right;
	InfixOperator(std::string_view symbol, std::unique_ptr<Node> left, std::unique_ptr<Node> right);
};
struct PrefixOperator : public Operator
{
	std::unique_ptr<Node> value;
	PrefixOperator(std::string_view symbol, std::unique_ptr<Node> operand);
	virtual void accept(Visitor &v) override
	{
		v.visit(*this);
	}
};
struct PostfixOperator : public Operator
{
	std::unique_ptr<Node> value;
	PostfixOperator(std::string_view symbol, std::unique_ptr<Node> operand);
	virtual void accept(Visitor &v) override
	{
		v.visit(*this);
	}
};
