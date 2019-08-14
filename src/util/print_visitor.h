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
		explicit PrintVisitor(bool verbose = false);

		// general
		void visit(Invalid &node);

		// expressions
		void visit(NumberLiteral &node);
		void visit(StringLiteral &node);
		void visit(BooleanLiteral &node);
		void visit(Identifier &node);
		void visit(FunctionCall &node);
		void visit(InfixOperator &node);
		void visit(PrefixOperator &node);
		void visit(PostfixOperator &node);

		// statements
		void visit(Block &node);
		void visit(Program &node);
		void visit(VariableDef &node);

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
