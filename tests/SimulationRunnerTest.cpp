#include "SimulationRunner.h"

#include <catch2/catch_test_macros.hpp>

#include <sstream>

using namespace Sim;
// clang-format off
auto exampleInput = std::stringstream{
R"(4 8 4 6
.......#
..#....#
.###...#
.....###
G=ub(B)
B=ub(m)lib(l)(m)
H=ib()(mmHllmll)
I=III
1 1 w
G
1 1 e
G
2 2 n
G
4 1 s
ib(lib()(mmm))(mmmm)
1 1 e
H
2 2 s
I)"
};

auto exampleOutput = std::stringstream{
R"(1 1 w
inf
1 1 w
4 4 e
1 4
inf)"
};
//clang-format on

TEST_CASE("Simulate whole example Input", "[SimulationRunner]") {
	runSimulation(exampleInput, std::cout);

	REQUIRE(1 == 1);
}