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

	// main

	void PrintVisitor::visit(Program &node)
	{
		print(str.stringify(node));
		tab_level++;
		for(const auto &stmt : node.statements)
		{
			stmt->accept(*this);
		}
		tab_level--;
	}

	// statements

	void PrintVisitor::visit(ExprStatement &node)
	{
		print(str.stringify(node));
		tab_level++;
		node.expr->accept(*this);
		tab_level--;
	}
	void PrintVisitor::visit(VariableDef &node)
	{
		print(str.stringify(node));
		tab_level++;
		node.name->accept(*this);
		if(node.type) node.type->accept(*this);
		if(node.value) node.value->accept(*this);
		tab_level--;
	}
	void PrintVisitor::visit(FunctionDef &node)
	{
		print(str.stringify(node));
		tab_level++;
		node.name->accept(*this);
		for(const auto &param : node.parameters)
		{
			param->accept(*this);
		}
		node.return_type->accept(*this);
		node.body->accept(*this);
		tab_level--;
	}

	// expressions

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
		tab_level--;
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
	void PrintVisitor::visit(ReturnExpr &node)
	{
		print(str.stringify(node));
		tab_level++;
		node.value->accept(*this);
		tab_level--;
	}

	// general

	void PrintVisitor::visit(Invalid &node)
	{
		print(str.stringify(node));
	}
	void PrintVisitor::visit(Block &node)
	{
		print(str.stringify(node));
		tab_level++;
		for(const auto &stmt : node.statements)
		{
			stmt->accept(*this);
		}
		tab_level--;
	}
	void PrintVisitor::visit(Parameter &node)
	{
		print(str.stringify(node));
		tab_level++;
		node.name->accept(*this);
		node.type->accept(*this);
		tab_level--;
	}
	void PrintVisitor::visit(Type &node)
	{
		print(str.stringify(node));
	}
}
