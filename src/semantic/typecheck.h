#pragma once

#include "log.h"
#include "util/stringifier.h"
#include "ast/visitor.h"
#include "ast/node.h"
#include "semantic/type.h"

class TypeChecker : public Visitor
{
public:
	TypeChecker(std::string_view file, std::string_view source);
	bool failed() const;

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
