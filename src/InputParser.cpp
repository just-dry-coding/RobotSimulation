#include "InputParser.h"
#include "StringUtilities.h"

#include <stdexcept>
#include <limits>

unsigned stou(std::string const& s) {
	unsigned long value = std::stoul(s);
	if (value > std::numeric_limits<unsigned int>::max()) {
		throw std::out_of_range("Value is out of range for unsigned int");
	}
	return static_cast<unsigned int>(value);
}

InputParser::InputParser(std::istream& input) {
	auto const lines = getInputLines(input);
	auto it = std::begin(lines);
	auto const metaInfo = parseFirstLine(lines[0]);
	++it; // adjust iterator after each parse
	_grid = parseGrid2D(metaInfo.gridRows, metaInfo.gridCols, it);
	it += metaInfo.gridRows;
	_procedures = parseProcedures(metaInfo.nProcedures, it);
	it += metaInfo.nProcedures;
	_programs = parsePrograms(metaInfo.nPrograms, it);
}

std::vector<std::string> InputParser::getInputLines(std::istream& input) {
	std::vector<std::string> lines;
	std::string line;
	while (std::getline(input, line)) {
		lines.push_back(line);
	}
	return lines;
}

InputParser::MetaInfo InputParser::parseFirstLine(std::string const& line) {
	auto const tokens = string_utils::splitString(line, ' ');

	return MetaInfo{
		stou(tokens[0]), stou(tokens[1]), stou(tokens[2]), stou(tokens[3]) };
}

Grid2D InputParser::parseGrid2D(unsigned lineRows, unsigned lineCols, LinesIterator gridBegin) {
	// add 2 to each dimension to account for the border
	auto grid2d = Grid2D(lineRows + 2, lineCols + 2);
	for (auto r = 0; r < lineRows; ++r) {
		for (auto c = 0; c < lineCols; ++c) {
			auto it = (*(gridBegin + r));
			auto ele = it[c];
			grid2d(r + 1, c + 1) = GridCellState(ele);
		}
	}
	return grid2d;
}

Procedures InputParser::parseProcedures(unsigned nProcedures, LinesIterator proceduresBegin) {
	std::map<char, std::string> procedures;
	for (auto it = proceduresBegin; it != (proceduresBegin + nProcedures); ++it) {
		auto const tokens = string_utils::splitString(*it, '=');
		procedures.try_emplace(tokens[0][0], tokens[1]);
	}
	return procedures;
}

Programs InputParser::parsePrograms(unsigned nPrograms, LinesIterator programsBegin) {
	std::vector<Program> programs;
	programs.reserve(nPrograms);

	for (auto it = programsBegin; it != (programsBegin + (nPrograms * 2)); it += 2) {
		auto const poseTokens = string_utils::splitString(*it, ' ');

		programs.push_back(
			{ Pose2D{stou(poseTokens[0]), stou(poseTokens[1]), Direction(poseTokens[2][0])},
			 *(it + 1) });
	}

	return programs;
}
