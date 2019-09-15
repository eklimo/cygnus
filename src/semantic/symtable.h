#pragma once

#include <unordered_map>
#include <memory>

#include "log.h"
#include "util/stringifier.h"
#include "ast/visitor.h"
#include "ast/node.h"
#include "symdata.h"

class SymbolTable : public Visitor
{
public:
	SymbolTable(std::string_view file, std::string_view source);
	bool failed() const;

	void enter_scope();
	void exit_scope();
	void define(Token token, Node *const node);
	std::shared_ptr<SymbolData> find(Token token);

	// main
	void visit(Program &node) override;

	// statements
	void visit(ExprStatement &node) override;
	void visit(VariableDef &node) override;
	void visit(FunctionDef &node) override;

	// expressions
	void visit(NumberLiteral &node) override;
	void visit(StringLiteral &node) override;
	void visit(BooleanLiteral &node) override;
	void visit(UnitLiteral &node) override;
	void visit(Identifier &node) override;
	void visit(FunctionCall &node) override;
	void visit(InfixOperator &node) override;
	void visit(PrefixOperator &node) override;
	void visit(PostfixOperator &node) override;
	void visit(ReturnExpr &node) override;
	void visit(IfExpr &node) override;
	void visit(WhileExpr &node) override;

	// general
	void visit(Invalid &node) override;
	void visit(Block &node) override;
	void visit(Parameter &node) override;
	void visit(Type &node) override;

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
