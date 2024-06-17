#include "SimulationRunner.h"
#include "RobotSimulator.h"
#include "InputParser.h"

void Sim::runSimulation(std::istream& input, std::ostream& os) {
	auto const inputParser = InputParser(input);

	auto simulator = RobotSimulator(inputParser.procedures(), inputParser.grid());
	for (auto const& program : inputParser.programs()) {
		auto res = simulator.runProgram(program);


		if (std::holds_alternative<Pose2D>(res)) {
			auto pose = std::get<Pose2D>(res);
			os << pose.x << ' ' << pose.y << ' ' << static_cast<char>(pose.d) << '\n';
		}
		else {
			os << "inf\n";
		}
	}
}