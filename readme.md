# RoboSimulation

RoboSimulation is a C++ library that allows you to run multiple programs provided by a
 std\::istream and writes their results to a provided std::ostream. It includes three
 build targets: SimulationRunnerLib, RobotSimulator, and tests.

## Build Targets

### SimulationRunnerLib

This build target creates a static library for other projects with the public header `SimulationRunner.h`.

### RobotSimulator

This build target creates an executable that takes a path to a file as an argument and runs the 
RoboSimulation with it.

### Tests

This build target runs the unit tests for the library.

