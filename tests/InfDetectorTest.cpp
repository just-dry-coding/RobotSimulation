#include "InfDetector.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Create Inf Detector", "[InfDetector]")
{
	auto infDetector = InfDetector();

	REQUIRE(&infDetector != nullptr);
}

TEST_CASE("Detect Simple Infinity", "[InfDetector]")
{
	auto infDetector = InfDetector();

	auto res = infDetector.addEntry("I", 1u, Pose2D{1u, 1u, Direction::n});

	REQUIRE(std::holds_alternative<InfDetector::None>(res));

	res = infDetector.addEntry("I", 1u, Pose2D{1u, 1u, Direction::n});

	REQUIRE(std::holds_alternative<InfDetector::Inf>(res));
}

TEST_CASE("Return Cached Result", "[InfDetector]")
{
	auto infDetector = InfDetector();

	auto res = infDetector.addEntry("I", 1u, Pose2D{1u, 1u, Direction::n});

	REQUIRE(std::holds_alternative<InfDetector::None>(res));

	infDetector.addStackFrameResult("I", Pose2D{1u, 1u, Direction::n}, Pose2D{2u, 2u, Direction::s});
	res = infDetector.addEntry("I", 1u, Pose2D{1u, 1u, Direction::n});

	REQUIRE(std::holds_alternative<Pose2D>(res));
	REQUIRE(std::get<Pose2D>(res) == Pose2D{2u, 2u, Direction::s});
}