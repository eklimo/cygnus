#pragma once

#include "log.h"
#include "ast/visitor.h"
#include "ast/node.h"

#include <string>
#include <sstream>

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
