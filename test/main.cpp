#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "../src/syntax/lexer.h"

TEST_CASE("lexer")
{
	const auto &tokens = Lexer::tokenize("");
	CHECK(tokens.size() == 0);
}
