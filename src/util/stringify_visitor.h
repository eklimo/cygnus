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

		virtual void visit(Invalid &node);

		virtual void visit(NumberLiteral &node);
		virtual void visit(StringLiteral &node);
		virtual void visit(BooleanLiteral &node);
		virtual void visit(Identifier &node);
		virtual void visit(FunctionCall &node);

		virtual void visit(InfixOperator &node);
		virtual void visit(PrefixOperator &node);
		virtual void visit(PostfixOperator &node);

		std::string stringify(Node &node);
	private:
		std::stringstream value;
		const bool verbose;

		std::string get();
	};
}
