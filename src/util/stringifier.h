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
		// main
		void visit(Program &node);

		// statements
		void visit(ExprStatement &node);
		void visit(VariableDef &node);
		void visit(FunctionDef &node);

		// expressions
		void visit(NumberLiteral &node);
		void visit(StringLiteral &node);
		void visit(BooleanLiteral &node);
		void visit(UnitLiteral &node);
		void visit(Identifier &node);
		void visit(FunctionCall &node);
		void visit(InfixOperator &node);
		void visit(PrefixOperator &node);
		void visit(PostfixOperator &node);
		void visit(ReturnExpr &node);
		void visit(IfExpr &node);
		void visit(WhileExpr &node);

		// general
		void visit(Invalid &node);
		void visit(Block &node);
		void visit(Parameter &node);
		void visit(Type &node);

		std::string stringify(Node &node);
	private:
		std::stringstream value;
		std::string get();
	};
}