#pragma once

#include <string>

#include "../ast/visitor.h"
#include "../ast/node.h"
#include "stringify_visitor.h"
#include "../log.h"

namespace Util
{
	class PrintVisitor : public Visitor
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

	private:
		unsigned tab_level = 0;
		StringifyVisitor str;
		const std::string prefix_tab() const;

		template<typename... Args>
		inline void print(Args &&... args) const
		{
			Logger::get().debug(std::string(3 * tab_level, ' '), std::forward<Args>(args)...);
		}
	};
}
