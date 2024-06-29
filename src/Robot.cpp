#include "Robot.h"
#include <cassert>

Robot::Robot(Pose2D const& initalPose, Grid2D const& grid) : _currentPose(initalPose), _grid(grid) {}

void Robot::turnLeft() {
	auto& currentDirection = _currentPose.d;
	switch (currentDirection) {
	case Direction::n:
		currentDirection = Direction::w;
		return;
	case Direction::w:
		currentDirection = Direction::s;
		return;
	case Direction::s:
		currentDirection = Direction::e;
		return;
	case Direction::e:
		currentDirection = Direction::n;
		return;
	}
}

Pose2D getNewPose(Pose2D const& p) {
	switch (p.d) {
	case Direction::n:
		return { p.x - 1, p.y, p.d };
	case Direction::w:
		return { p.x, p.y - 1, p.d };
	case Direction::s:
		return { p.x + 1, p.y, p.d };
	case Direction::e:
		return { p.x, p.y + 1, p.d };
	default:
		assert(false && "Unhandled direction in getNewPose");
		return {};
	}
};

bool Robot::isBlocked() const {
	auto const suggestedPose = getNewPose(_currentPose);
	return (_grid(suggestedPose.x, suggestedPose.y) == GridCellState::b);
}

// blocked state could be cached for better performance
void Robot::moveForward() {
	auto const suggestedPose = getNewPose(_currentPose);
	if (_grid(suggestedPose.x, suggestedPose.y) == GridCellState::b)
		return;
	_currentPose = suggestedPose;
}
