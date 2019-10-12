#include "noderange.h"

#include "syntax/token.h"

#include <iostream>

namespace Util
{
	NodeRange::NodeRange()
	{
	}

	NodeRange::NodeRange(Node *const node)
	{
		node->accept(*this);
	}

	void NodeRange::token_range(const Token &token)
	{
		bool is_string = token.type == TokenType::String;

		Util::FileLocation _begin
		(
		    token.location.line,
		    token.location.column
		);
		begin = _begin < begin ? _begin : begin;

		Util::FileLocation _end
		(
		    token.location.line,
		    token.location.column + token.value.length() + (is_string ? 2 : 0)
		);
		end = _end > end ? _end : end;
	}

	// main

	void NodeRange::visit(Program &node)
	{
		for(const auto &stmt : node.statements)
		{
			stmt->accept(*this);
		}
	}

	// statements

	void NodeRange::visit(ExprStatement &node)
	{
		node.expr->accept(*this);
	}
	void NodeRange::visit(VariableDef &node)
	{
	}
	void NodeRange::visit(FunctionDef &node)
	{
	}

	// expressions

	void NodeRange::visit(NumberLiteral &node)
	{
		token_range(node.token);
	}
	void NodeRange::visit(StringLiteral &node)
	{
		token_range(node.token);
	}
	void NodeRange::visit(BooleanLiteral &node)
	{
		token_range(node.token);
	}
	void NodeRange::visit(UnitLiteral &node)
	{
		token_range(node.token);
	}
	void NodeRange::visit(Identifier &node)
	{
		token_range(node.token);
	}
	void NodeRange::visit(FunctionCall &node)
	{
		node.name->accept(*this);
		token_range(node.rparen);
	}
	void NodeRange::visit(InfixOperator &node)
	{
		node.left->accept(*this);
		node.right->accept(*this);
	}
	void NodeRange::visit(PrefixOperator &node)
	{
		token_range(node.token);
		node.operand->accept(*this);
	}
	void NodeRange::visit(PostfixOperator &node)
	{
		token_range(node.token);
		node.operand->accept(*this);
	}
	void NodeRange::visit(GroupExpr &node)
	{
		token_range(node.lparen);
		node.expr->accept(*this);
		token_range(node.rparen);
	}
	void NodeRange::visit(ReturnExpr &node)
	{
		token_range(node.return_keyword);
		if(node.value)
			node.value->accept(*this);
	}
	void NodeRange::visit(IfExpr &node)
	{
		token_range(node.if_keyword);
		node.if_branch->accept(*this);
		if(node.else_branch)
		{
			token_range(node.else_keyword);
			node.else_branch->accept(*this);
		}
	}
	void NodeRange::visit(WhileExpr &node)
	{
		token_range(node.while_keyword);
		node.body->accept(*this);
	}

	// general

	void NodeRange::visit(Invalid &node)
	{
	}
	void NodeRange::visit(Block &node)
	{
		token_range(node.lbrace);
		token_range(node.rbrace);
	}
	void NodeRange::visit(Parameter &node)
	{
		node.name->accept(*this);
		node.type->accept(*this);
	}
	void NodeRange::visit(Type &node)
	{
		token_range(node.token);
	}
}
