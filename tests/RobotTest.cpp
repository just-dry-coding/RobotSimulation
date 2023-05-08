#include "Robot.h"
#include "InputParser.h"

#include <catch2/catch_test_macros.hpp>

auto const testGridStrings = std::vector<std::string>{".###", "...#", "...#", "...#"};
auto const testGrid = InputParser::parseGrid2D(4, 4, std::begin(testGridStrings));

TEST_CASE("Create Robot at initalPose", "[Robot]")
{
	auto initialPose = Pose2D{1, 1, Direction::n};
	auto robot = Robot(initialPose, testGrid);

	REQUIRE(&robot != nullptr);
	REQUIRE(robot.currentPose() == initialPose);
}

TEST_CASE("Turn Robot to the left", "[Robot]")
{
	auto initialPose = Pose2D{1, 1, Direction::n};
	auto robot = Robot(initialPose, testGrid);

	REQUIRE(robot.currentPose().d == Direction::n);
	REQUIRE(robot.isBlocked());

	robot.turnLeft();

	REQUIRE(robot.currentPose().d == Direction::w);
	REQUIRE(robot.isBlocked());

	robot.turnLeft();

	REQUIRE(robot.currentPose().d == Direction::s);
	REQUIRE(!robot.isBlocked());

	robot.turnLeft();

	REQUIRE(robot.currentPose().d == Direction::e);
	REQUIRE(robot.isBlocked());
}

TEST_CASE("Robot moves forward", "[Robot]")
{
	auto initialPose = Pose2D{3, 3, Direction::n};
	auto robot = Robot(initialPose, testGrid);
	
	REQUIRE(robot.currentPose().x == 3);

	robot.moveForward();

	REQUIRE(robot.currentPose().x == 2);
}

TEST_CASE("Robot moves against obstacel", "[Robot]")
{
	auto initialPose = Pose2D{2, 2, Direction::n};
	auto robot = Robot(initialPose, testGrid);

	REQUIRE(robot.currentPose().x == 2);
	REQUIRE(robot.isBlocked());

	robot.moveForward();

	REQUIRE(robot.currentPose().x == 2);
}

TEST_CASE("Robot moves out of bounds", "[Robot]")
{
	auto initialPose = Pose2D{2, 2, Direction::w};
	auto robot = Robot(initialPose, testGrid);

	REQUIRE(robot.currentPose().y == 2);
	REQUIRE(!robot.isBlocked());

	robot.moveForward();

	REQUIRE(robot.currentPose().y == 1);
	REQUIRE(robot.isBlocked());

	robot.moveForward();

	REQUIRE(robot.currentPose().y == 1);
}
