#include "catch.hpp"
#include <Utilities/Strings.h>

TEST_CASE("Split String works correctly with a present character")
{
	auto parts = SplitString("a|b|c", '|');

	REQUIRE(parts.at(0) == "a");
	REQUIRE(parts.at(1) == "b");
	REQUIRE(parts.at(2) == "c");
}

TEST_CASE("Split String works with character not present")
{
	auto parts = SplitString("abc", '|');
	REQUIRE(parts.size() == 1);
	REQUIRE(parts.at(0) == "abc");
}

TEST_CASE("Split string returns an empty vector for an empty string")
{
	auto parts = SplitString("", ',');
	REQUIRE(parts.empty());
}