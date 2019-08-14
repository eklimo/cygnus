#pragma once

#include <string>
#include <sstream>

#include "../ast/visitor.h"
#include "../ast/node.h"
#include "../log.h"

namespace Util
{
	class StringifyVisitor : public Visitor
	{
	public:
		explicit StringifyVisitor(bool verbose = false);

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

		std::string stringify(Node &node);
	private:
		std::stringstream value;
		const bool verbose;

		std::string get();
	};
}
