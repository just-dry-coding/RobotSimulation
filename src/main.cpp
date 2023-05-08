#include "SimulationRunner.h"

#include <fstream>

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: " << argv[0] << " <filepath>\n";
		return 1;
	}

	std::ifstream file(argv[1]);
	if (!file.is_open())
	{
		std::cerr << "Failed to open file: " << argv[1] << '\n';
		return 1;
	}

	Sim::runSimulationRunner(file, std::cout);

	return 0;
}
