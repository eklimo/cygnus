#include "treeprinter.h"

namespace Util
{
	// main

	void TreePrinter::visit(Program &node)
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

	void TreePrinter::visit(ExprStatement &node)
	{
		print(str.stringify(node));
		tab_level++;
		node.expr->accept(*this);
		tab_level--;
	}
	void TreePrinter::visit(VariableDef &node)
	{
		print(str.stringify(node));
		tab_level++;
		if(node.type) node.type->accept(*this);
		if(node.value) node.value->accept(*this);
		tab_level--;
	}
	void TreePrinter::visit(FunctionDef &node)
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

	void TreePrinter::visit(NumberLiteral &node)
	{
		print(str.stringify(node));
	}
	void TreePrinter::visit(StringLiteral &node)
	{
		print(str.stringify(node));
	}
	void TreePrinter::visit(BooleanLiteral &node)
	{
		print(str.stringify(node));
	}
	void TreePrinter::visit(UnitLiteral &node)
	{
		print(str.stringify(node));
	}
	void TreePrinter::visit(Identifier &node)
	{
		print(str.stringify(node));
		if(node.symbol) print("  -> ", str.stringify(*node.symbol->node));
	}
	void TreePrinter::visit(FunctionCall &node)
	{
		print(str.stringify(node));
		if(node.name->symbol) print("  -> ", str.stringify(*node.name->symbol->node));
		tab_level++;
		for(const auto &arg : node.arguments)
		{
			arg->accept(*this);
		}
		tab_level--;
	}
	void TreePrinter::visit(InfixOperator &node)
	{
		print(str.stringify(node));
		tab_level++;
		node.left->accept(*this);
		node.right->accept(*this);
		tab_level--;
	}
	void TreePrinter::visit(PrefixOperator &node)
	{
		print(str.stringify(node));
		tab_level++;
		node.operand->accept(*this);
		tab_level--;
	}
	void TreePrinter::visit(PostfixOperator &node)
	{
		print(str.stringify(node));
		tab_level++;
		node.operand->accept(*this);
		tab_level--;
	}
	void TreePrinter::visit(GroupExpr &node)
	{
		// print(str.stringify(node));
		// tab_level++;
		node.expr->accept(*this);
		// tab_level--;
	}
	void TreePrinter::visit(ReturnExpr &node)
	{
		print(str.stringify(node));
		tab_level++;
		if(node.value) node.value->accept(*this);
		tab_level--;
	}
	void TreePrinter::visit(IfExpr &node)
	{
		print(str.stringify(node));
		tab_level++;
		node.condition->accept(*this);
		node.if_branch->accept(*this);
		if(node.else_branch) node.else_branch->accept(*this);
		tab_level--;
	}
	void TreePrinter::visit(WhileExpr &node)
	{
		print(str.stringify(node));
		tab_level++;
		node.condition->accept(*this);
		node.body->accept(*this);
		tab_level--;
	}

	// general

	void TreePrinter::visit(Invalid &node)
	{
		print(str.stringify(node));
	}
	void TreePrinter::visit(Block &node)
	{
		print(str.stringify(node));
		tab_level++;
		for(const auto &stmt : node.statements)
		{
			stmt->accept(*this);
		}
		tab_level--;
	}
	void TreePrinter::visit(Parameter &node)
	{
		print(str.stringify(node));
		tab_level++;
		node.type->accept(*this);
		tab_level--;
	}
	void TreePrinter::visit(Type &node)
	{
		print(str.stringify(node));
	}
}
