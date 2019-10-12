#pragma once

#include "log.h"
#include "stringifier.h"
#include "ast/visitor.h"
#include "ast/node.h"

#include <string>

namespace Util
{
	class TreePrinter : public Visitor
	{
	public:
#include "ast/visitorincl"

	private:
		Stringifier str;
		unsigned tab_level = 1;

		template<typename... Args>
		inline void print(Args &&... args) const
		{
			Logger::get().debug(std::string(3 * tab_level, ' '), std::forward<Args>(args)...);
		}
	};
}
