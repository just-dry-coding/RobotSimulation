#include "StringUtilities.h"

#include <catch2/catch_test_macros.hpp>

using namespace string_utils;

TEST_CASE("Split string at delimiter", "[StringUtilities]")
{
	auto s = std::string{"1 2 3 4"};

	auto tokens = splitString(s, ' ');

	REQUIRE(tokens[0] == "1");
	REQUIRE(tokens[1] == "2");
	REQUIRE(tokens[2] == "3");
	REQUIRE(tokens[3] == "4");
}
