#pragma once
#include "Datatypes.h"

#include <map>
#include <tuple>
#include <optional>

// The InfDetector class is used to detect infinite loops in the robot program.
// It keeps track of the state of the robot and program.
// If the robot and the program are in a previous state, an infinite loop is detected.
// Robot state consists of the pose of the robot.
// Program state consists of the code that was executed and the position of the code on the stack.
// To avoid simulating the same codes and robot pose combinations again, resulting robot poses are cached.
class InfDetector {
	struct SimulationState {
		unsigned stackPos;
		Pose2D pose;
		std::optional<Pose2D> resultPose;
	};

	std::multimap<std::string, SimulationState, std::less<>> _simulationStateMap;

public:
	std::optional<Pose2D> addEntry(std::string const& code, unsigned pos, Pose2D const& pose);

	void addStackFrameResult(std::string const& code, Pose2D const& initialPose, Pose2D const& resultPose);
};
