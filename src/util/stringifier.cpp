#include "stringifier.h"

namespace Util
{
	std::string Stringifier::stringify(Node &node)
	{
		node.accept(*this);
		auto temp = value.str();
		value.str(std::string());
		return temp;
	}

	// main

	void Stringifier::visit(Program &node)
	{
		value << "Program";
	}

	// statements

	void Stringifier::visit(ExprStatement &node)
	{
		value << "Expression statement";
	}
	void Stringifier::visit(VariableDef &node)
	{
		value << "Variable definition '" << node.name->token.value << "'";
	}
	void Stringifier::visit(FunctionDef &node)
	{
		value << "Function definition '" << node.name->token.value << "'";
	}

	// expressions

	void Stringifier::visit(NumberLiteral &node)
	{
		value << "Number '" << node.token.value << "'";
	}
	void Stringifier::visit(StringLiteral &node)
	{
		value << "String '" << node.token.value << "'";
	}
	void Stringifier::visit(BooleanLiteral &node)
	{
		value << "Boolean '" << node.token.value << "'";
	}
	void Stringifier::visit(UnitLiteral &node)
	{
		value << "Unit '" << node.token.value << "'";
	}
	void Stringifier::visit(Identifier &node)
	{
		value << "Identifier '" << node.token.value << "'";
	}
	void Stringifier::visit(FunctionCall &node)
	{
		value << "Function call '" << node.name->token.value << "'";
	}
	void Stringifier::visit(InfixOperator &node)
	{
		value << "Infix operator '" << node.token.value << "'";
	}
	void Stringifier::visit(PrefixOperator &node)
	{
		value << "Prefix operator '" << node.token.value << "'";
	}
	void Stringifier::visit(PostfixOperator &node)
	{
		value << "Postfix operator '" << node.token.value << "'";
	}
	void Stringifier::visit(ReturnExpr &node)
	{
		value << "Return expression";
	}
	void Stringifier::visit(IfExpr &node)
	{
		value << "If expression";
	}
	void Stringifier::visit(WhileExpr &node)
	{
		value << "While expression";
	}

	// general

	void Stringifier::visit(Invalid &node)
	{
		value << "Invalid";
	}
	void Stringifier::visit(Block &node)
	{
		value << "Block";
	}
	void Stringifier::visit(Parameter &node)
	{
		value << "Parameter '" << node.name->token.value << "'";
	}
	void Stringifier::visit(Type &node)
	{
		value << "Type '" << node.token.value << "'";
	}
}
