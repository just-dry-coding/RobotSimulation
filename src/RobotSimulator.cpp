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

void RobotSimulator::addStackFrame(std::string const& code) {
	_procedureStack.push({ code, _robot.currentPose(), 0 });
}

void RobotSimulator::addInfinityEntry(StackFrame const& stackFrame) {
	auto res = _infDetector.addEntry(stackFrame.code, stackFrame.posInStackFrame, _robot.currentPose());
	if (res.has_value()) {
		throw res.value();
	}
}

void RobotSimulator::performBranch(StackFrame& stackFrame) {
	auto it = std::cbegin(stackFrame.code) + stackFrame.posInStackFrame;
	auto endIt = std::cend(stackFrame.code);

	auto openingIt = std::find(it, endIt, '(') + 1;
	auto firstCondClose = findCorrespondingClosingBracket(openingIt, endIt);
	auto secondCondClose = findCorrespondingClosingBracket(firstCondClose + 2, endIt);

	stackFrame.posInStackFrame += (secondCondClose + 1) - it;

	if (evalCondition(Condition(*(it + 1)), _robot)) {
		if (firstCondClose == openingIt)
			return; // nop for empty bracket
		addStackFrame({ openingIt, firstCondClose });
	}
	else {
		if (firstCondClose == secondCondClose)
			return; // nop for empty bracket
		addStackFrame({ firstCondClose + 2, secondCondClose });
	}
}

void RobotSimulator::performLoop(StackFrame& stackFrame) {
	auto it = std::cbegin(stackFrame.code) + stackFrame.posInStackFrame;
	auto endIt = std::cend(stackFrame.code);

	auto openingIt = std::find(it, endIt, '(') + 1;
	auto closeIt = findCorrespondingClosingBracket(openingIt, endIt);

	if (!evalCondition(Condition(*(it + 1)), _robot)) {
		std::string outerLoop{ it, closeIt + 1 };
		std::string innterLoop{ openingIt, closeIt };
		addStackFrame(outerLoop);
		addStackFrame(innterLoop);
	}
	else {
		stackFrame.posInStackFrame += (closeIt + 1) - it;
	}
}

void RobotSimulator::executeSimpleCommand(StackFrame& stackFrame) {
	switch (SimpleCommands(stackFrame.code[stackFrame.posInStackFrame])) {
	case SimpleCommands::move:
		_robot.moveForward();
		++stackFrame.posInStackFrame;
		break;
	case SimpleCommands::turnLeft:
		_robot.turnLeft();
		++stackFrame.posInStackFrame;
		break;
	case SimpleCommands::branch:
		performBranch(stackFrame);
		break;
	case SimpleCommands::loop:
		performLoop(stackFrame);
		break;
	}
}

void RobotSimulator::resolveProcedure(StackFrame& stackFrame) {
	++stackFrame.posInStackFrame;
	addStackFrame({ _procedures.at(stackFrame.code[stackFrame.posInStackFrame - 1]) });
}

void RobotSimulator::doNextStep() {
	auto& currentStackFrame = _procedureStack.top();
	if (islower(currentStackFrame.code[currentStackFrame.posInStackFrame])) {
		executeSimpleCommand(currentStackFrame);
	}
	else {
		resolveProcedure(currentStackFrame);
	}
	addInfinityEntry(currentStackFrame);
}

void RobotSimulator::popStack() {
	if (!_procedureStack.size())
		return;
	auto& currentStackFrame = _procedureStack.top();
	while (currentStackFrame.code.size() <= currentStackFrame.posInStackFrame) {
		_infDetector.addStackFrameResult(
			currentStackFrame.code, currentStackFrame.initalPose, _robot.currentPose());
		_infDetector.addStackFrameResult(
			currentStackFrame.code, _robot.currentPose(), _robot.currentPose());
		_procedureStack.pop();
		if (!_procedureStack.size()) {
			break;
		}
		currentStackFrame = _procedureStack.top();
	}
}

Pose2D RobotSimulator::runStack() {
	while (!_procedureStack.empty()) {
		try {
			doNextStep();
		}
		catch (Pose2D const& pose) {
			_robot.setPose(pose);
			_procedureStack.pop();
		}
		popStack();
	}
	return _robot.currentPose();
}

Pose2D RobotSimulator::runProgram(Program const& program) {
	_robot.setPose(program.initPose);
	_procedureStack = std::stack<StackFrame>{ };
	_procedureStack.push({ program.code, program.initPose });
	_infDetector = InfDetector{}; // todo: check if better to initialize simulator for every program

	return runStack();
}

RobotSimulator::RobotSimulator(Procedures const& procedures, Grid2D const& grid)
	: _robot(Robot{ Pose2D{0u, 0u, Direction::n}, grid }), _procedures(procedures) {}
