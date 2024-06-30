#pragma once
#include "Datatypes.h"

#include <map>
#include <tuple>
#include <optional>

// The InfDetector class is used to detect infinite loops in the robot program.
// It works by keeping track of each procedure that was executed in the simulator.
// If the same procedure ends up in the exact same state (posInStackFrame and pose),
// without having a resultPose, it means that the procedure is stuck in an infinite loop.
class InfDetector {
	struct SimulationState {
		unsigned posInStackFrame;
		Pose2D pose;
		std::optional<Pose2D> resultPose;
	};

	std::multimap<std::string, SimulationState, std::less<>> _simulationStateMap;

public:
	std::optional<Pose2D> addEntry(std::string const& code, unsigned posInStackFrame, Pose2D const& pose);

	void addStackFrameResult(std::string const& code, Pose2D const& initialPose, Pose2D const& resultPose);
};
