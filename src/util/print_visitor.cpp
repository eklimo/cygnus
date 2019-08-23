#include "print_visitor.h"

namespace Util
{
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
		if(node.type) node.type->accept(*this);
		if(node.value) node.value->accept(*this);
		tab_level--;
	}
	void PrintVisitor::visit(FunctionDef &node)
	{
		print(str.stringify(node));
		tab_level++;
		for(const auto &param : node.parameters)
		{
			param->accept(*this);
		}
		if(node.return_type) node.return_type->accept(*this);
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
	void PrintVisitor::visit(UnitLiteral &node)
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
		if(node.value) node.value->accept(*this);
		tab_level--;
	}
	void PrintVisitor::visit(IfExpr &node)
	{
		print(str.stringify(node));
		tab_level++;
		node.condition->accept(*this);
		node.if_branch->accept(*this);
		if(node.else_branch) node.else_branch->accept(*this);
		tab_level--;
	}
	void PrintVisitor::visit(WhileExpr &node)
	{
		print(str.stringify(node));
		tab_level++;
		node.condition->accept(*this);
		node.body->accept(*this);
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
		node.type->accept(*this);
		tab_level--;
	}
	void PrintVisitor::visit(Type &node)
	{
		print(str.stringify(node));
	}
}
