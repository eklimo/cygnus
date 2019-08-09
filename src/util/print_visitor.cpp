#include "print_visitor.h"

namespace Util
{
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
		print("Invalid");
	}

	void PrintVisitor::visit(NumberLiteral &node)
	{
		print("Number '", node.value, "'");
	}
	void PrintVisitor::visit(StringLiteral &node)
	{
		print("String '", node.value, "'");
	}
	void PrintVisitor::visit(BooleanLiteral &node)
	{
		print("Boolean '", node.value, "'");
	}
	void PrintVisitor::visit(Identifier &node)
	{
		print("Identifier '", node.value, "'");
	}

	void PrintVisitor::visit(InfixOperator &node)
	{
		print("Infix operator '", node.symbol, "'");
		tab_level++;
		node.left->accept(*this);
		node.right->accept(*this);
		tab_level--;
	}
	void PrintVisitor::visit(PrefixOperator &node)
	{
		print("Prefix operator '", node.symbol, "'");
		tab_level++;
		node.value->accept(*this);
		tab_level--;
	}
	void PrintVisitor::visit(PostfixOperator &node)
	{
		print("Postfix operator '", node.symbol, "'");
		tab_level++;
		node.value->accept(*this);
		tab_level--;
	}
}
