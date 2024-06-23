#include "RobotSimulator.h"

#include "StringUtilities.h"

#include <algorithm>
#include <cctype>
#include <functional>
#include <map>

using StrIt = std::string::const_iterator;

// combine control commands with action commands to align with specifications
enum class SimpleCommands {
	move = 'm',
	turnLeft = 'l',
	branch = 'i',
	loop = 'u'
};

enum class Condition {
	blocked = 'b',
	north = 'n',
	south = 's',
	east = 'e',
	west = 'w',
};

bool evalCondition(Condition const c, Robot const& r) {
	static const std::map<Condition, std::function<bool(const Robot&)>> conditionMap = {
		{Condition::blocked, [](const Robot& r) { return r.isBlocked(); }},
		{Condition::north, [](const Robot& r) { return r.currentPose().d == Direction::n; }},
		{Condition::south, [](const Robot& r) { return r.currentPose().d == Direction::s; }},
		{Condition::east, [](const Robot& r) { return r.currentPose().d == Direction::e; }},
		{Condition::west, [](const Robot& r) { return r.currentPose().d == Direction::w; }},
	};

	if (auto it = conditionMap.find(c); it != conditionMap.end()) {
		return it->second(r);
	}

	return true;
}

StrIt findCorrespondingClosingBracket(StrIt it, StrIt endIt) {
	StrIt closingIt;
	while (true) {
		closingIt = std::find(it, endIt, ')');
		if (std::find(it, closingIt, '(') == closingIt)
			return closingIt;
		it = closingIt + 1;
	}
}

void RobotSimulator::addStackEntry(std::string const& code) {
	_stack.push_back({ code, _robot.currentPose(), 0 });
}

void RobotSimulator::addInfinityEntry(StackEntry const& stackEntry) {
	auto res = _infDetector.addEntry(stackEntry.code, stackEntry.pos, _robot.currentPose());
	if (std::holds_alternative<InfDetector::Inf>(res))
		throw Inf{};
	if (std::holds_alternative<Pose2D>(res))
		throw std::get<Pose2D>(res);
}

void RobotSimulator::performBranch(StackEntry& stackEntry) {
	auto it = std::cbegin(stackEntry.code) + stackEntry.pos;
	auto endIt = std::cend(stackEntry.code);

	auto openingIt = std::find(it, endIt, '(') + 1;
	auto firstCondClose = findCorrespondingClosingBracket(openingIt, endIt);
	auto secondCondClose = findCorrespondingClosingBracket(firstCondClose + 2, endIt);

	stackEntry.pos += (secondCondClose + 1) - it;

	if (evalCondition(Condition(*(it + 1)), _robot)) {
		if (firstCondClose == openingIt)
			return; // nop for empty bracket
		addStackEntry({ openingIt, firstCondClose });
	}
	else {
		if (firstCondClose == secondCondClose)
			return; // nop for empty bracket
		addStackEntry({ firstCondClose + 2, secondCondClose });
	}
}

void RobotSimulator::performLoop(StackEntry& stackEntry) {
	auto it = std::cbegin(stackEntry.code) + stackEntry.pos;
	auto endIt = std::cend(stackEntry.code);

	auto openingIt = std::find(it, endIt, '(') + 1;
	auto closeIt = findCorrespondingClosingBracket(openingIt, endIt);

	if (!evalCondition(Condition(*(it + 1)), _robot)) {
		std::string outerLoop{ it, closeIt + 1 };
		std::string innterLoop{ openingIt, closeIt };
		addStackEntry(outerLoop);
		addStackEntry(innterLoop);
	}
	else {
		stackEntry.pos += (closeIt + 1) - it;
	}
}

void RobotSimulator::executeSimpleCommand(StackEntry& stackEntry) {
	switch (SimpleCommands(stackEntry.code[stackEntry.pos])) {
	case SimpleCommands::move:
		_robot.moveForward();
		++stackEntry.pos;
		break;
	case SimpleCommands::turnLeft:
		_robot.turnLeft();
		++stackEntry.pos;
		break;
	case SimpleCommands::branch:
		performBranch(stackEntry);
		break;
	case SimpleCommands::loop:
		performLoop(stackEntry);
		break;
	}
}

void RobotSimulator::resolveProcedure(StackEntry& stackEntry) {
	++stackEntry.pos;
	addStackEntry({ _procedures.at(stackEntry.code[stackEntry.pos - 1]) });
}

void RobotSimulator::doNextStep(size_t stackIndex) {
	auto& stackEntry = _stack[stackIndex];
	if (islower(stackEntry.code[stackEntry.pos])) {
		executeSimpleCommand(stackEntry);
	}
	else {
		resolveProcedure(stackEntry);
	}
	addInfinityEntry(_stack[stackIndex]);
}

void RobotSimulator::popStack() {
	if (!_stack.size())
		return;
	auto& currentStackFrame = _stack[_stack.size() - 1];
	while (currentStackFrame.code.size() <= currentStackFrame.pos) {
		_infDetector.addStackFrameResult(
			currentStackFrame.code, currentStackFrame.initalPose, _robot.currentPose());
		_infDetector.addStackFrameResult(
			currentStackFrame.code, _robot.currentPose(), _robot.currentPose());
		_stack.pop_back();
		if (!_stack.size()) {
			break;
		}
		currentStackFrame = _stack[_stack.size() - 1];
	}
}

Pose2D RobotSimulator::runStack() {
	while (_stack.size() > 0) {
		try {
			doNextStep(_stack.size() - 1);
		}
		catch (Pose2D const& pose) {
			_robot.setPose(pose);
			_stack.pop_back();
		}
		popStack();
	}
	return _robot.currentPose();
}

Pose2D RobotSimulator::runProgram(Program const& program) {
	_robot.setPose(program.initPose);
	_stack = Stack{ {program.code, program.initPose} };
	_infDetector = InfDetector{}; // todo: check if better to initialize simulator for every program

	return runStack();
}

RobotSimulator::RobotSimulator(Procedures const& procedures, Grid2D const& grid)
	: _robot(Robot{ Pose2D{0u, 0u, Direction::n}, grid }), _procedures(procedures) {}
