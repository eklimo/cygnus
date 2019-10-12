#pragma once

#include "log.h"
#include "util/stringifier.h"
#include "ast/visitor.h"
#include "ast/node.h"
#include "symdata.h"

#include <unordered_map>
#include <memory>

class SymbolTable : public Visitor
{
public:
#include "ast/visitorincl"

	SymbolTable(std::string_view file, std::string_view source);
	bool failed() const;

	void enter_scope();
	void exit_scope();
	void define(Token token, Node *const node);
	std::shared_ptr<SymbolData> find(Token token);

private:
	std::unordered_map<std::string_view, std::shared_ptr<SymbolData>> symbols;
	unsigned scope_level;

	std::string_view file, source;
	bool error;

	Util::Stringifier str;
	unsigned tab_level = 0;
	template<typename... Args>
	inline void print(Args &&... args) const
	{
		Logger::get().debug(std::string(3 * tab_level, ' '), std::forward<Args>(args)...);
	}
};
