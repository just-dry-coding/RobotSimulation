#pragma once

#include <Datatypes.h>

class Robot{
	Grid2D const& _grid;
	Pose2D _currentPose;
public:
	Robot(Pose2D const& initalPose, Grid2D const& grid);

	void turnLeft();

	void moveForward();

	bool isBlocked() const;

	Pose2D currentPose() const { return _currentPose; }

	void setPose(Pose2D const& pose) { _currentPose = pose; }
};