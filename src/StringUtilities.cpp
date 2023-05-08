#include "StringUtilities.h"

#include <sstream>

std::vector<std::string> string_utils::splitString(std::string const& s, char delimiter)
{
	auto ss = std::stringstream{s};
	std::string token;
	std::vector<std::string> tokens;

	while (getline(ss, token, delimiter))
	{
		tokens.push_back(token);
	}

	return tokens;
}
