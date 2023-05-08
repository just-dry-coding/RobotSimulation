#pragma once
#include "Datatypes.h"

#include <map>
#include <variant>
#include <tuple>
#include <optional>

class InfDetector
{
	// multimap containing all previous states for all previous stackframes
	// if a result is already known for the stackframe + state combination
	// then should be put into the optional Pose
	std::multimap<std::string, std::tuple<unsigned, Pose2D, std::optional<Pose2D>>> _stackStateSuccessMap;

public:
	struct Inf
	{
	};
	struct None
	{
	};
	std::variant<Pose2D, Inf, None> addEntry(std::string const& code, unsigned pos, Pose2D const& pose);

	void addStackFrameResult(std::string const& code, Pose2D const& initialPose, Pose2D const& resultPose);
};
