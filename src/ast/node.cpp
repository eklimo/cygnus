#include "node.h"

void Invalid::accept(Visitor &v)
{
	v.visit(*this);
}

Value::Value(std::string_view _value)
	: value(_value)
{}
void NumberLiteral::accept(Visitor &v)
{
	v.visit(*this);
}
void StringLiteral::accept(Visitor &v)
{
	v.visit(*this);
}
void BooleanLiteral::accept(Visitor &v)
{
	v.visit(*this);
}
void Identifier::accept(Visitor &v)
{
	v.visit(*this);
}
FunctionCall::FunctionCall(std::unique_ptr<Node> _name, std::vector<std::unique_ptr<Node>> _arguments)
	: name(std::move(_name)), arguments(std::move(_arguments))
{}
void FunctionCall::accept(Visitor &v)
{
	v.visit(*this);
}

Operator::Operator(std::string_view _symbol)
	: symbol(_symbol)
{}
InfixOperator::InfixOperator(std::string_view _symbol, std::unique_ptr<Node> _left, std::unique_ptr<Node> _right)
	: Operator(_symbol), left(std::move(_left)), right(std::move(_right))
{}
void InfixOperator ::accept(Visitor &v)
{
	v.visit(*this);
}
PrefixOperator::PrefixOperator(std::string_view _symbol, std::unique_ptr<Node> _operand)
	: Operator(_symbol), operand(std::move(_operand))
{}
void PrefixOperator::accept(Visitor &v)
{
	v.visit(*this);
}
PostfixOperator::PostfixOperator(std::string_view _symbol, std::unique_ptr<Node> _operand)
	: Operator(_symbol), operand(std::move(_operand))
{}
void PostfixOperator::accept(Visitor &v)
{
	v.visit(*this);
}
