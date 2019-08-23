#pragma once

#include <memory>

// #include "ast/node.h"
class Node;

struct SymbolData
{
	SymbolData(std::shared_ptr<SymbolData> next, unsigned scope_level, Node *const node);
	const std::shared_ptr<SymbolData> next;
	const unsigned scope_level;

	Node *const node;
};
