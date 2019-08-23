#include "symdata.h"

SymbolData::SymbolData(std::shared_ptr<SymbolData> next, unsigned scope_level, Node *const node)
	: next(next),
	  scope_level(scope_level),
	  node(node)
{
}
