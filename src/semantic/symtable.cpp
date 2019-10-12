#include "symtable.h"

#include "util/error.h"

SymbolTable::SymbolTable(std::string_view file, std::string_view source)
	: scope_level(0),
	  file(file),
	  source(source),
	  error(false)
{
}

bool SymbolTable::failed() const
{
	return error;
}

void SymbolTable::enter_scope()
{
	scope_level++;
	tab_level++;
}

void SymbolTable::exit_scope()
{
	// delete entries at current level
	for(auto it = symbols.begin(); it != symbols.end();)
	{
		if(it->second->scope_level == scope_level)
		{
			// replace entry with next
			if(it->second->next)
			{
				it->second = it->second->next;
			}
			// erase entry
			else
			{
				it = symbols.erase(it);
				continue;
			}
		}
		it++;
	}

	scope_level--;
	tab_level--;
}

void SymbolTable::define(Token token, Node *const node)
{
	auto id = token.value;
	print("Define '", id, "' = ", str.stringify(*node));

	auto it = symbols.find(id);

	if(it != symbols.end() && it->second->scope_level == scope_level)
	{
		error = true;
		Util::Error::At(
		    token,
		    "symbol '", id, "' is already defined"
		).print(file, source);
		return;
	}

	symbols[id] =
	    std::make_shared<SymbolData>
	    (
	        it == symbols.end() ? nullptr : symbols[id],
	        scope_level,
	        node
	    );
}

std::shared_ptr<SymbolData> SymbolTable::find(Token token)
{
	auto id = token.value;

	auto it = symbols.find(id);
	if(it == symbols.end())
	{
		print("Find '", id, "' -> undefined");
		error = true;
		Util::Error::At(
		    token,
		    "symbol '", id, "' is not defined"
		).print(file, source);
		return nullptr;
	}

	print("Find '", id, "' -> ", str.stringify(*it->second->node));
	return it->second;
}

// main

void SymbolTable::visit(Program &node)
{
	for(const auto &stmt : node.statements)
	{
		stmt->accept(*this);
	}
}

// statements

void SymbolTable::visit(ExprStatement &node)
{
	node.expr->accept(*this);
}
void SymbolTable::visit(VariableDef &node)
{
	if(node.value) node.value->accept(*this);
	define(node.name->token, node.name.get());
}
void SymbolTable::visit(FunctionDef &node)
{
	define(node.name->token, node.name.get());
	enter_scope();
	for(const auto &param : node.parameters)
	{
		param->accept(*this);
	}
	for(const auto &stmt : node.body->statements)
	{
		stmt->accept(*this);
	}
	exit_scope();
}

// expressions

void SymbolTable::visit(NumberLiteral &node) {}
void SymbolTable::visit(StringLiteral &node) {}
void SymbolTable::visit(BooleanLiteral &node) {}
void SymbolTable::visit(UnitLiteral &node) {}
void SymbolTable::visit(Identifier &node)
{
	node.symbol = find(node.token);
}
void SymbolTable::visit(FunctionCall &node)
{
	node.name->accept(*this);
	for(const auto &arg : node.arguments)
	{
		arg->accept(*this);
	}
}
void SymbolTable::visit(InfixOperator &node)
{
	node.left->accept(*this);
	node.right->accept(*this);
}
void SymbolTable::visit(PrefixOperator &node)
{
	node.operand->accept(*this);
}
void SymbolTable::visit(PostfixOperator &node)
{
	node.operand->accept(*this);
}
void SymbolTable::visit(GroupExpr &node)
{
	node.expr->accept(*this);
}
void SymbolTable::visit(ReturnExpr &node)
{
	if(node.value) node.value->accept(*this);
}
void SymbolTable::visit(IfExpr &node)
{
	node.condition->accept(*this);
	node.if_branch->accept(*this);
	if(node.else_branch) node.else_branch->accept(*this);
}
void SymbolTable::visit(WhileExpr &node)
{
	node.condition->accept(*this);
	node.body->accept(*this);
}

// general

void SymbolTable::visit(Invalid &node) {}
void SymbolTable::visit(Block &node)
{
	enter_scope();
	for(const auto &stmt : node.statements)
	{
		stmt->accept(*this);
	}
	exit_scope();
}
void SymbolTable::visit(Parameter &node)
{
	define(node.name->token, node.name.get());
}
void SymbolTable::visit(Type &node) {}
