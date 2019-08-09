#include "print_visitor.h"

namespace Util
{
	PrintVisitor::PrintVisitor(bool verbose)
		: str(verbose)
	{}

	const std::string PrintVisitor::prefix_tab() const
	{
		std::string s = "";
		for(unsigned i = 0; i < tab_level; i++)
		{
			s += "  ";
		}
		return s;
	}

	void PrintVisitor::visit(Invalid &node)
	{
		print(str.stringify(node));
	}

	void PrintVisitor::visit(NumberLiteral &node)
	{
		print(str.stringify(node));
	}
	void PrintVisitor::visit(StringLiteral &node)
	{
		print(str.stringify(node));
	}
	void PrintVisitor::visit(BooleanLiteral &node)
	{
		print(str.stringify(node));
	}
	void PrintVisitor::visit(Identifier &node)
	{
		print(str.stringify(node));
	}
	void PrintVisitor::visit(FunctionCall &node)
	{
		print(str.stringify(node));
		tab_level++;
		for(const auto &arg : node.arguments)
		{
			arg->accept(*this);
		}
		tab_level++;
	}

	void PrintVisitor::visit(InfixOperator &node)
	{
		print(str.stringify(node));
		tab_level++;
		node.left->accept(*this);
		node.right->accept(*this);
		tab_level--;
	}
	void PrintVisitor::visit(PrefixOperator &node)
	{
		print(str.stringify(node));
		tab_level++;
		node.operand->accept(*this);
		tab_level--;
	}
	void PrintVisitor::visit(PostfixOperator &node)
	{
		print(str.stringify(node));
		tab_level++;
		node.operand->accept(*this);
		tab_level--;
	}
}
