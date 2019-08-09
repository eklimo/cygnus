#pragma once

#include <vector>
#include <string_view>

#include "token.h"

namespace Lexer
{
	std::vector<Token> tokenize(std::string_view input);
}
