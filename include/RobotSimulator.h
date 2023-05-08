﻿#pragma once

#include "Robot.h"
#include "InfDetector.h"

#include <variant>

class RobotSimulator
{
	struct StackEntry
	{
		std::string code;
		Pose2D initalPose; 
		unsigned pos = 0;
	};

	using Stack = std::vector<StackEntry>;
	Robot _robot;
	Procedures _procedures;
	Stack _stack;
	InfDetector _infDetector;


public:
	struct Inf
	{
	};

	RobotSimulator(Procedures const& procedures, Grid2D const& grid);

	std::variant<Pose2D, Inf> runProgram(Program const& program);

private:
	using StrIt = std::string::const_iterator;
	std::variant<Pose2D, Inf> runStack();
	void doNextStep(size_t stackIndex);
	void executeSimpleCommand(StackEntry&);
	void addStackEntry(std::string const& code);
	void performBranch(StackEntry& stackEntry);
	void performLoop(StackEntry& stackEntry);
	void resolveProcedure(StackEntry& stackEntry);
	void popStack();
	void addInfinityEntry(StackEntry const& code);
};