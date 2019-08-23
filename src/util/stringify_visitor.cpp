#include "stringify_visitor.h"

namespace Util
{
	std::string StringifyVisitor::get()
	{
		std::string str = value.str();
		value.str(std::string());
		return str;
	}

	std::string StringifyVisitor::stringify(Node &node)
	{
		node.accept(*this);
		return get();
	}

	// main

	void StringifyVisitor::visit(Program &node)
	{
		value << "Program";
	}

	// statements

	void StringifyVisitor::visit(ExprStatement &node)
	{
		value << "Expression statement";
	}
	void StringifyVisitor::visit(VariableDef &node)
	{
		value << "Variable definition '" << node.name->token.value << "'";
	}
	void StringifyVisitor::visit(FunctionDef &node)
	{
		value << "Function definition '" << node.name->token.value << "'";
	}

	// expressions

	void StringifyVisitor::visit(NumberLiteral &node)
	{
		value << "Number '" << node.token.value << "'";
	}
	void StringifyVisitor::visit(StringLiteral &node)
	{
		value << "String '" << node.token.value << "'";
	}
	void StringifyVisitor::visit(BooleanLiteral &node)
	{
		value << "Boolean '" << node.token.value << "'";
	}
	void StringifyVisitor::visit(UnitLiteral &node)
	{
		value << "Unit '" << node.token.value << "'";
	}
	void StringifyVisitor::visit(Identifier &node)
	{
		value << "Identifier '" << node.token.value << "'";
	}
	void StringifyVisitor::visit(FunctionCall &node)
	{
		value << "Function call '" << node.name->token.value << "'";
	}
	void StringifyVisitor::visit(InfixOperator &node)
	{
		value << "Infix operator '" << node.symbol << "'";
	}
	void StringifyVisitor::visit(PrefixOperator &node)
	{
		value << "Prefix operator '" << node.symbol << "'";
	}
	void StringifyVisitor::visit(PostfixOperator &node)
	{
		value << "Postfix operator '" << node.symbol << "'";
	}
	void StringifyVisitor::visit(ReturnExpr &node)
	{
		value << "Return expression";
	}
	void StringifyVisitor::visit(IfExpr &node)
	{
		value << "If expression";
	}
	void StringifyVisitor::visit(WhileExpr &node)
	{
		value << "While expression";
	}

	// general

	void StringifyVisitor::visit(Invalid &node)
	{
		value << "Invalid";
	}
	void StringifyVisitor::visit(Block &node)
	{
		value << "Block";
	}
	void StringifyVisitor::visit(Parameter &node)
	{
		value << "Parameter '" << node.name->token.value << "'";
	}
	void StringifyVisitor::visit(Type &node)
	{
		value << "Type '" << node.value << "'";
	}
}
