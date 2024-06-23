#include "SimulationRunner.h"
#include "RobotSimulator.h"
#include "InputParser.h"

void Sim::runSimulation(std::istream& input, std::ostream& os) {
	auto const inputParser = InputParser(input);

	auto simulator = RobotSimulator(inputParser.procedures(), inputParser.grid());
	for (auto const& program : inputParser.programs()) {
		try {
			auto pose = simulator.runProgram(program);
			os << pose.x << ' ' << pose.y << ' ' << static_cast<char>(pose.d) << '\n';
		}
		catch (RobotSimulator::Inf const&) {
			os << "inf\n";
		}
	}
}