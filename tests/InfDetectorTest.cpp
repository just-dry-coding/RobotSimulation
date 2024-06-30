#include "InfDetector.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Detect Simple Infinity", "[InfDetector]") {
	auto infDetector = InfDetector();

	auto res = infDetector.addEntry("I", 1u, Pose2D{ 1u, 1u, Direction::n });

	REQUIRE(res.has_value() == false);

	REQUIRE_THROWS_AS(infDetector.addEntry("I", 1u, Pose2D{ 1u, 1u, Direction::n }), Inf);
}

TEST_CASE("Return Cached Result", "[InfDetector]") {
	auto infDetector = InfDetector();

	auto res = infDetector.addEntry("I", 1u, Pose2D{ 1u, 1u, Direction::n });

	REQUIRE(res.has_value() == false);

	infDetector.addStackFrameResult("I", Pose2D{ 1u, 1u, Direction::n }, Pose2D{ 2u, 2u, Direction::s });
	res = infDetector.addEntry("I", 1u, Pose2D{ 1u, 1u, Direction::n });

	REQUIRE(res.has_value());
	REQUIRE(res.value() == Pose2D{ 2u, 2u, Direction::s });
}