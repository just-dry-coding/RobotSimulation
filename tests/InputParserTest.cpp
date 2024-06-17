#include "InputParser.h"

#include <catch2/catch_test_macros.hpp>

#include <sstream>


TEST_CASE("Read input line by line", "[InputParser]") {
	auto line1 = std::string{ "1 2 3 4" };
	auto line2 = std::string{ ".....#" };
	auto input_stream = std::stringstream();
	input_stream << line1 << '\n' << line2;

	auto lines = InputParser::getInputLines(input_stream);

	REQUIRE(lines[0] == line1);
	REQUIRE(lines[1] == line2);
}

TEST_CASE("Parse first line", "[InputParser]") {
	auto firstLine = std::string{ "4 8 4 6" };

	auto parsedLine = InputParser::parseFirstLine(firstLine);

	REQUIRE(parsedLine.gridRows == 4);
	REQUIRE(parsedLine.gridCols == 8);
	REQUIRE(parsedLine.nProcedures == 4);
	REQUIRE(parsedLine.nPrograms == 6);
}

TEST_CASE("Parse grid", "[InputParser]") {
	auto gridStrings = std::vector<std::string>{ {"...#"}, {"##.#"}, {"..#."} };

	auto grid =
		InputParser::parseGrid2D(gridStrings.size(), gridStrings[0].size(), std::begin(gridStrings));

	REQUIRE(grid(1, 1) == GridCellState::f);
	REQUIRE(grid(1, 4) == GridCellState::b);
	REQUIRE(grid(3, 3) == GridCellState::b);
	REQUIRE(grid(2, 3) == GridCellState::f);
	REQUIRE(grid(0, 0) == GridCellState::b);
	REQUIRE(grid(4, 5) == GridCellState::b);
}

TEST_CASE("Parse procedures", "[InputParser]") {
	auto procedureStrings =
		std::vector<std::string>{ {"G=ub(B)"}, {"B=ub(m)lib(l)(m)"}, {"H=ib()(mmHllmll)"}, {"I=III"} };

	auto procedures =
		InputParser::parseProcedures(procedureStrings.size(), std::begin(procedureStrings));

	REQUIRE(procedures['G'] == "ub(B)");
	REQUIRE(procedures['B'] == "ub(m)lib(l)(m)");
	REQUIRE(procedures['H'] == "ib()(mmHllmll)");
	REQUIRE(procedures['I'] == "III");
}

TEST_CASE("Parse programs", "[InputParser]") {
	auto programStrings = std::vector<std::string>{
		{"1 1 w"},
		{"G"},
		{"1 1 e"},
		{"G"},
		{"2 2 n"},
		{"G"},
		{"4 1 s"},
		{"ib(lib()(mmm))(mmmm)"},
		{"1 1 e"},
		{"H"},
		{"2 2 s"},
		{"I"} };

	auto programs = InputParser::parsePrograms(programStrings.size() / 2, std::begin(programStrings));

	REQUIRE(programs.size() == (programStrings.size() / 2));
	REQUIRE(programs[0].code == "G");
	REQUIRE(programs[5].initPose == Pose2D{ 2, 2, Direction('s') });
}

TEST_CASE("Parse whole input", "[InputParser]") {
	auto programStrings = std::vector<std::string>{
		{"1 1 w"},
		{"G"},
		{"1 1 e"},
		{"G"},
		{"2 2 n"},
		{"G"},
		{"4 1 s"},
		{"ib(lib()(mmm))(mmmm)"},
		{"1 1 e"},
		{"H"},
		{"2 2 s"},
		{"I"} };

	auto programs = InputParser::parsePrograms(programStrings.size() / 2, std::begin(programStrings));

	REQUIRE(programs.size() == (programStrings.size() / 2));
	REQUIRE(programs[0].code == "G");
	REQUIRE(programs[5].initPose == Pose2D{ 2, 2, Direction('s') });
}

TEST_CASE("Parse combined input", "[InputParser]") {
	auto input_stream = std::stringstream{
		// clang-format off
R"(1 4 1 1
.#..
A=ub(B)
1 1 w
G)"
//clang-format on
	};
	auto inputParser = InputParser(input_stream);

	REQUIRE(inputParser.grid().rows() == 3);
	REQUIRE(inputParser.grid().cols() == 6);
	REQUIRE(inputParser.grid()(1, 1) == GridCellState::f);
	REQUIRE(inputParser.grid()(1, 2) == GridCellState::b);
	REQUIRE(inputParser.procedures().at('A') == "ub(B)");
	REQUIRE(inputParser.programs()[0].initPose == Pose2D{ 1,1,Direction::w });
	REQUIRE(inputParser.programs()[0].code == "G");
}
