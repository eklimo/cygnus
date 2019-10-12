#pragma once

#include "ast/visitor.h"
#include "ast/node.h"

#include <climits>

namespace Util
{
	class NodeRange : public Visitor
	{
	public:
#include "ast/visitorincl"

		NodeRange();
		explicit NodeRange(Node *const node);

		Util::FileLocation begin = { UINT_MAX, UINT_MAX };
		Util::FileLocation end = { 0, 0 };

	private:
		void token_range(const Token &token);
	};
}
