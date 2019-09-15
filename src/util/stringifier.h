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
#include "ast/visitorincl"

		std::string stringify(Node &node);

	private:
		std::stringstream value;
	};
}
