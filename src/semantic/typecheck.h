#pragma once

#include "log.h"
#include "util/stringifier.h"
#include "ast/visitor.h"
#include "ast/node.h"
#include "semantic/type.h"

class TypeChecker : public Visitor
{
public:
#include "ast/visitorincl"

	TypeChecker(std::string_view file, std::string_view source);
	bool failed() const;

private:
	std::string_view file, source;
	bool error;

	DataType type;
	DataType get_type(Node &node);
	DataType check_infix(const Token &token, const Expression *const left, const DataType &left_type, const Expression *const right, const DataType &right_type);
	DataType check_prefix(const Token &token, const Expression *const operand, const DataType &operand_type);
	DataType check_postfix(const Token &token, const Expression *const operand, const DataType &operand_type);

	Util::Stringifier str;
	unsigned tab_level = 0;
	template<typename... Args>
	inline void print(Args &&... args) const
	{
		Logger::get().debug(std::string(3 * tab_level, ' '), std::forward<Args>(args)...);
	}
};
