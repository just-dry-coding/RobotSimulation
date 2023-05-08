#include "InfDetector.h"

std::variant<Pose2D, InfDetector::Inf, InfDetector::None>
InfDetector::addEntry(std::string const& code, unsigned pos, Pose2D const& pose)
{
	auto range = _stackStateSuccessMap.equal_range(code);
	if (range.first == range.second)
	{
		_stackStateSuccessMap.insert({code, {pos, pose, std::nullopt}});
		return {None{}};
	}

	for (auto it = range.first; it != range.second; ++it)
	{
		auto const ele = *it;
		if (pose == std::get<1>(ele.second) && pos == std::get<0>(ele.second))
		{
			auto const optionalResultPose = std::get<2>(ele.second);
			if (std::nullopt == optionalResultPose)
			{
				return {Inf{}};
			}
			return {*optionalResultPose};
		}
	}
	_stackStateSuccessMap.insert({code, {pos, pose, std::nullopt}});
}

void InfDetector::addStackFrameResult(std::string const& code, Pose2D const& initialPose, Pose2D const& resultPose){
	auto range = _stackStateSuccessMap.equal_range(code);
	for (auto it = range.first; it != range.second; ++it)
	{
		auto& ele = *it;
		if (initialPose == std::get<1>(ele.second))
		{
			std::get<2>(ele.second) = resultPose;
		}
	}
}
