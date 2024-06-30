#pragma once

#include "Robot.h"
#include "InfDetector.h"
#include <stack>


class RobotSimulator {
	// StackFrame are like mini-call stacks that are executed in the simulator
	// multiple commands from either the program or procedure
	// the robot pose, when the stack frame was created
	// the current position in the stack frame
	struct StackFrame {
		std::string code;
		Pose2D initalPose;
		unsigned posInStackFrame = 0;
	};
	Robot _robot;
	Procedures _procedures;
	std::stack<StackFrame> _procedureStack;
	InfDetector _infDetector;


public:
	RobotSimulator(Procedures const& procedures, Grid2D const& grid);

	Pose2D runProgram(Program const& program);

private:
	using StrIt = std::string::const_iterator;
	Pose2D runStack();
	void doNextStep();
	void executeSimpleCommand(StackFrame&);
	void addStackFrame(std::string const& code);
	void performBranch(StackFrame& stackFrame);
	void performLoop(StackFrame& stackFrame);
	void resolveProcedure(StackFrame& stackFrame);
	void popStack();
	void addInfinityEntry(StackFrame const& code);
};
