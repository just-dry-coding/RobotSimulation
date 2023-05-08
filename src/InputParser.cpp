#include "InputParser.h"
#include "StringUtilities.h"

InputParser::InputParser(std::istream& input)
{
	auto const lines = getInputLines(input);
	LinesIterator it = std::begin(lines);
	auto const metaInfo = parseFirstLine(lines[0]);
	++it; // adjust iterator after each parse
	_grid = parseGrid2D(metaInfo.gridRows, metaInfo.gridCols, it);
	it += metaInfo.gridRows;
	_procedures = parseProcedures(metaInfo.nProcedures, it);
	it += metaInfo.nProcedures;
	_programs = parsePrograms(metaInfo.nPrograms, it);
}

std::vector<std::string> InputParser::getInputLines(std::istream& input)
{
	std::vector<std::string> lines;
	std::string line;
	while (std::getline(input, line))
	{
		lines.push_back(line);
	}
	return lines;
}

InputParser::MetaInfo InputParser::parseFirstLine(std::string const& line)
{
	auto const tokens = string_utils::splitString(line, ' ');

	return MetaInfo{
		std::stoul(tokens[0]), std::stoul(tokens[1]), std::stoul(tokens[2]), std::stoul(tokens[3])};
}

Grid2D InputParser::parseGrid2D(unsigned lineRows, unsigned lineCols, LinesIterator gridBegin)
{
	auto grid2d = Grid2D(lineRows + 2, lineCols + 2);
	for (auto r = 0; r < lineRows; ++r)
	{
		for (auto c = 0; c < lineCols; ++c)
		{
			auto it = (*(gridBegin + r));
			auto ele = it[c];
			grid2d(r + 1, c + 1) = GridCellState(ele);
		}
	}
	return grid2d;
}

Procedures InputParser::parseProcedures(unsigned nProcedures, LinesIterator proceduresBegin)
{
	std::map<char, std::string> procedures;
	for (auto it = proceduresBegin; it != (proceduresBegin + nProcedures); ++it)
	{
		auto const tokens = string_utils::splitString(*it, '=');
		procedures.insert({tokens[0][0], tokens[1]});
	}
	return procedures;
}

Programs InputParser::parsePrograms(unsigned nPrograms, LinesIterator programsBegin)
{
	std::vector<Program> programs;
	programs.reserve(nPrograms);

	for (auto it = programsBegin; it != (programsBegin + (nPrograms * 2)); it += 2)
	{
		auto const poseTokens = string_utils::splitString(*it, ' ');

		programs.push_back(
			{Pose2D{stoul(poseTokens[0]), stoul(poseTokens[1]), Direction(poseTokens[2][0])},
			 *(it + 1)});
	}

	return programs;
}
