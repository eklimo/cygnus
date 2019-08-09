#pragma once

// #include "node.h"
struct Invalid;

struct NumberLiteral;
struct StringLiteral;
struct BooleanLiteral;
struct Identifier;

struct InfixOperator;
struct PrefixOperator;
struct PostfixOperator;

class Visitor
{
public:
	virtual void visit(Invalid &node) = 0;

	virtual void visit(NumberLiteral &node) = 0;
	virtual void visit(StringLiteral &node) = 0;
	virtual void visit(BooleanLiteral &node) = 0;
	virtual void visit(Identifier &node) = 0;

	virtual void visit(InfixOperator &node) = 0;
	virtual void visit(PrefixOperator &node) = 0;
	virtual void visit(PostfixOperator &node) = 0;
};
