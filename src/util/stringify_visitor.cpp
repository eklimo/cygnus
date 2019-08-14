#include "stringify_visitor.h"

namespace Util
{
	StringifyVisitor::StringifyVisitor(bool _verbose)
		: verbose(_verbose)
	{
	}

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

	// general
	void StringifyVisitor::visit(Invalid &node)
	{
		value << (verbose ? "Invalid" : "inv");
	}

	// expressions
	void StringifyVisitor::visit(NumberLiteral &node)
	{
		if(verbose)
			value << "Number '" << node.value << "'";
		else
			value << node.value;
	}
	void StringifyVisitor::visit(StringLiteral &node)
	{
		if(verbose)
			value << "String '" << node.value << "'";
		else
			value << node.value;
	}
	void StringifyVisitor::visit(BooleanLiteral &node)
	{
		if(verbose)
			value << "Boolean '" << node.value << "'";
		else
			value << node.value;
	}
	void StringifyVisitor::visit(Identifier &node)
	{
		if(verbose)
			value << "Identifier '" << node.value << "'";
		else
			value << node.value;
	}
	void StringifyVisitor::visit(FunctionCall &node)
	{
		auto name = stringify(*node.name.get());

		if(verbose)
			value << "Function call '" << name << "'";
		else
			value << name << "()";
	}
	void StringifyVisitor::visit(InfixOperator &node)
	{
		if(verbose)
			value << "Infix operator '" << node.symbol << "'";
		else
			value << node.symbol;
	}
	void StringifyVisitor::visit(PrefixOperator &node)
	{
		if(verbose)
			value << "Prefix operator '" << node.symbol << "'";
		else
			value << node.symbol;
	}
	void StringifyVisitor::visit(PostfixOperator &node)
	{
		if(verbose)
			value << "Postfix operator '" << node.symbol << "'";
		else
			value << node.symbol;
	}

	// statements
	void StringifyVisitor::visit(Block &node)
	{
		if(verbose)
			value << "Block";
		else
			value << "<block>";
	}
	void StringifyVisitor::visit(Program &node)
	{
		if(verbose)
			value << "Program";
		else
			value << "<program>";
	}
	void StringifyVisitor::visit(VariableDef &node)
	{
		auto name = stringify(*node.name.get());

		if(verbose)
			value << "Variable definition '" << name << "'";
		else
			value << "var";
	}
}
