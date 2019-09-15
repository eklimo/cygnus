#pragma once

#include <memory>

// #include "ast/node.h"
class Node;

#include "semantic/type.h"

struct SymbolData
{
	SymbolData(std::shared_ptr<SymbolData> next, unsigned scope_level, Node *const node, DataType type = DataType::Invalid);
	const std::shared_ptr<SymbolData> next;
	const unsigned scope_level;

	Node *const node;
	DataType type;
};
