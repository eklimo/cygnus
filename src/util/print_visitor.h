#pragma once

#include <string>

#include "../ast/visitor.h"
#include "../ast/node.h"
#include "../log.h"

namespace Util
{
	class PrintVisitor : public Visitor
	{
	public:
		virtual void visit(Invalid &node);

		virtual void visit(NumberLiteral &node);
		virtual void visit(StringLiteral &node);
		virtual void visit(BooleanLiteral &node);
		virtual void visit(Identifier &node);

		virtual void visit(InfixOperator &node);
		virtual void visit(PrefixOperator &node);
		virtual void visit(PostfixOperator &node);

	private:
		unsigned tab_level = 0;
		const std::string prefix_tab() const;

		template<typename... Args>
		inline void print(Args &&... args) const
		{
			Logger::get().debug(prefix_tab(), std::forward<Args>(args)...);
		}
	};
}
