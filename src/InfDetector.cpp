#include "InfDetector.h"

std::optional<Pose2D>
InfDetector::addEntry(std::string const& code, unsigned posInStackFrame, Pose2D const& pose) {
	auto [start, end] = _simulationStateMap.equal_range(code);

	for (auto it = start; it != end; ++it) {
		auto& [_, state] = *it;
		if (pose == state.pose && posInStackFrame == state.posInStackFrame) {
			if (state.resultPose) {
				return state.resultPose;
			}
			throw Inf{};
		}
	}

	_simulationStateMap.insert({ code, {posInStackFrame, pose, std::nullopt} });
	return std::nullopt;
}

void InfDetector::addStackFrameResult(std::string const& code, Pose2D const& initialPose, Pose2D const& resultPose) {
	auto [start, end] = _simulationStateMap.equal_range(code);
	for (auto it = start; it != end; ++it) {
		auto& [_, state] = *it;
		if (initialPose == state.pose) {
			state.resultPose = resultPose;
		}
	}
}
