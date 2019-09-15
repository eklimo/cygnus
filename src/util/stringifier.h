#pragma once

#include <string>
#include <sstream>

#include "log.h"
#include "ast/visitor.h"
#include "ast/node.h"

namespace Util
{
	class Stringifier : public Visitor
	{
	public:
		std::string stringify(Node &node);

		// main
		void visit(Program &node) override;

		// statements
		void visit(ExprStatement &node) override;
		void visit(VariableDef &node) override;
		void visit(FunctionDef &node) override;

		// expressions
		void visit(NumberLiteral &node) override;
		void visit(StringLiteral &node) override;
		void visit(BooleanLiteral &node) override;
		void visit(UnitLiteral &node) override;
		void visit(Identifier &node) override;
		void visit(FunctionCall &node) override;
		void visit(InfixOperator &node) override;
		void visit(PrefixOperator &node) override;
		void visit(PostfixOperator &node) override;
		void visit(ReturnExpr &node) override;
		void visit(IfExpr &node) override;
		void visit(WhileExpr &node) override;

		// general
		void visit(Invalid &node) override;
		void visit(Block &node) override;
		void visit(Parameter &node) override;
		void visit(Type &node) override;

	private:
		std::stringstream value;
	};
}
