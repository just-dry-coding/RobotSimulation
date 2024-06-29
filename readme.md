# Robot Simulator

The Robot Simulator is a C++ application designed to simulate the movement of robots on a grid based on a series of programmed instructions. It reads input from a file, processes the instructions for each robot, and outputs their final positions on the grid.

## Features

- Parse and execute a series of instructions for robots moving on a grid.
- Detect infinite loops in robot instructions and report them.
- Output the final position of the robot after executing its instructions.

## Prerequisites

- CMake (version 3.15 or higher)
- A C++ compiler supporting C++17 (GCC, Clang, MSVC)
- Catch2 (for testing)

## Building the Project

To build the project, simply run the following command in the terminal:

```sh
make build
```

## Running the Simulator

You can run the simulator with:

```sh
./RobotSimulator <filepath>
```
