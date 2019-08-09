#include "node.h"

Value::Value(std::string_view _value)
	: value(_value)
{}

Operator::Operator(std::string_view _symbol)
	: symbol(_symbol)
{}

InfixOperator::InfixOperator(std::string_view _symbol, std::unique_ptr<Node> _left, std::unique_ptr<Node> _right)
	: Operator(_symbol), left(std::move(_left)), right(std::move(_right))
{}

PrefixOperator::PrefixOperator(std::string_view _symbol, std::unique_ptr<Node> _value)
	: Operator(_symbol), value(std::move(_value))
{}

PostfixOperator::PostfixOperator(std::string_view _symbol, std::unique_ptr<Node> _value)
	: Operator(_symbol), value(std::move(_value))
{}
