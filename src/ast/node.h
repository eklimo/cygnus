#pragma once

#include <string_view>
#include <memory>
#include <vector>

#include "visitor.h"

struct Node
{
	virtual void accept(Visitor &v) = 0;
};
struct Invalid: public Node
{
	void accept(Visitor &v) override;
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
	std::unique_ptr<Node> name;
	std::vector<std::unique_ptr<Node>> arguments;

	FunctionCall(std::unique_ptr<Node> name, std::vector<std::unique_ptr<Node>> arguments);

	void accept(Visitor &v) override;
};

struct Operator : public Node
{
	const std::string_view symbol;

	Operator(std::string_view symbol);
};
struct InfixOperator : public Operator
{
	std::unique_ptr<Node> left;
	std::unique_ptr<Node> right;

	InfixOperator(std::string_view symbol, std::unique_ptr<Node> left, std::unique_ptr<Node> right);

	void accept(Visitor &v) override;
};
struct PrefixOperator : public Operator
{
	std::unique_ptr<Node> operand;

	PrefixOperator(std::string_view symbol, std::unique_ptr<Node> operand);

	void accept(Visitor &v) override;
};
struct PostfixOperator : public Operator
{
	std::unique_ptr<Node> operand;

	PostfixOperator(std::string_view symbol, std::unique_ptr<Node> operand);

	void accept(Visitor &v) override;
};
