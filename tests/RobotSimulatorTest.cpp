#include "RobotSimulator.h"
#include "InputParser.h"

#include <catch2/catch_test_macros.hpp>

#include <sstream>

// clang-format off
auto sheetProgram3 = std::stringstream{
R"(4 8 2 1
.......#
..#....#
.###...#
.....###
G=ub(B)
B=ub(m)lib(l)(m)
2 2 n
G)"
};
//clang-format on

TEST_CASE("Simulate sheet program 3", "[RobotSimulator]")
{
	auto inputParser = InputParser(sheetProgram3);
	auto simulator = RobotSimulator(inputParser.procedures(), inputParser.grid());
	auto res = simulator.runProgram(inputParser.programs()[0]);

	REQUIRE(std::holds_alternative<Pose2D>(res));
	REQUIRE(std::get<Pose2D>(res) == Pose2D{1,1,Direction::w});
}

// clang-format off
auto sheetProgram4 = std::stringstream{
R"(4 8 2 1
.......#
..#....#
.###...#
.....###
G=ub(B)
B=ub(m)lib(l)(m)
4 1 s
ib(lib()(mmm))(mmmm))"
};
//clang-format on

TEST_CASE("Simulate sheet program 4", "[RobotSimulator]")
{
	auto inputParser = InputParser(sheetProgram4);
	auto simulator = RobotSimulator(inputParser.procedures(), inputParser.grid());
	auto res = simulator.runProgram(inputParser.programs()[0]);

	REQUIRE(std::holds_alternative<Pose2D>(res));
	REQUIRE(std::get<Pose2D>(res) == Pose2D{4,4,Direction::e});
}

// clang-format off
auto minimalProgramInput = std::stringstream{
R"(2 2 0 1
..
..
1 1 w
mlm)"
};
//clang-format on

TEST_CASE("Simulate program with minmalProgram", "[RobotSimulator]")
{
	auto inputParser = InputParser(minimalProgramInput);
	auto simulator = RobotSimulator(inputParser.procedures(), inputParser.grid());
	auto res = simulator.runProgram(inputParser.programs()[0]);

	REQUIRE(std::holds_alternative<Pose2D>(res));
	REQUIRE(std::get<Pose2D>(res) == Pose2D{2,1,Direction::s});
}

// clang-format off
auto branchProgramInput = std::stringstream{
R"(2 2 0 1
..
..
1 1 w
ib(l)(mlm))"
};
//clang-format on

TEST_CASE("Simulate program with branching condition true", "[RobotSimulator]")
{
	auto inputParser = InputParser(branchProgramInput);
	auto simulator = RobotSimulator(inputParser.procedures(), inputParser.grid());
	auto res = simulator.runProgram(inputParser.programs()[0]);

	REQUIRE(std::holds_alternative<Pose2D>(res));
	REQUIRE(std::get<Pose2D>(res) == Pose2D{1,1,Direction::s});
}

// clang-format off
auto branchSecondProgramInput = std::stringstream{
R"(2 2 0 1
..
..
1 1 s
ib(l)(mlm))"
};
//clang-format on

TEST_CASE("Simulate program with branching condition false", "[RobotSimulator]")
{
	auto inputParser = InputParser(branchSecondProgramInput);
	auto simulator = RobotSimulator(inputParser.procedures(), inputParser.grid());
	auto res = simulator.runProgram(inputParser.programs()[0]);

	REQUIRE(std::holds_alternative<Pose2D>(res));
	REQUIRE(std::get<Pose2D>(res) == Pose2D{2,2,Direction::e});
}

// clang-format off
auto loopProgramInput = std::stringstream{
R"(2 2 0 1
..
..
1 1 s
ub(l))"
};
//clang-format on

TEST_CASE("Simulate program with looping", "[RobotSimulator]")
{
	auto inputParser = InputParser(loopProgramInput);
	auto simulator = RobotSimulator(inputParser.procedures(), inputParser.grid());
	auto res = simulator.runProgram(inputParser.programs()[0]);

	REQUIRE(std::holds_alternative<Pose2D>(res));
	REQUIRE(std::get<Pose2D>(res) == Pose2D{1,1,Direction::n});
}

// clang-format off
auto detectSimpleInf = std::stringstream{
R"(3 3 0 1
...
...
...
2 2 s
ub(l))"
};
//clang-format on

TEST_CASE("Detect simple infinity", "[RobotSimulator]")
{
	auto inputParser = InputParser(detectSimpleInf);
	auto simulator = RobotSimulator(inputParser.procedures(), inputParser.grid());
	auto res = simulator.runProgram(inputParser.programs()[0]);

	REQUIRE(std::holds_alternative<RobotSimulator::Inf>(res));
}

// clang-format off
auto simpleProcedure = std::stringstream{
R"(3 3 1 1
...
...
...
G=ub(l)
2 2 s
ub(l))"
};
//clang-format on

TEST_CASE("Run simple procedure", "[RobotSimulator]")
{
	auto inputParser = InputParser(simpleProcedure);
	auto simulator = RobotSimulator(inputParser.procedures(), inputParser.grid());
	auto res = simulator.runProgram(inputParser.programs()[0]);

	REQUIRE(std::holds_alternative<RobotSimulator::Inf>(res));
}
