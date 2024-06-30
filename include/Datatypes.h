#pragma once

#include <vector>
#include <string>
#include <map>

struct Inf {
};

enum class Direction {
	n = 'n',
	s = 's',
	e = 'e',
	w = 'w'
};

struct Pose2D {
	unsigned x;
	unsigned y;
	Direction d;

	inline bool operator==(Pose2D const rhs) const {
		return x == rhs.x && y == rhs.y && d == rhs.d;
	}
};

enum class GridCellState {
	b = '#',
	f = '.'
};

class Grid2D {
	unsigned _rows;
	unsigned _cols;
	std::vector<GridCellState> _data;

public:
	Grid2D() = default;

	Grid2D(unsigned rows, unsigned cols) : _rows(rows), _cols(cols), _data(rows* cols, GridCellState::b) {}

	GridCellState& operator()(unsigned row, unsigned col) { return _data[row * _cols + col]; }
	GridCellState const& operator()(unsigned row, unsigned col) const { return _data[row * _cols + col]; }

	unsigned rows() const { return _rows; }
	unsigned cols() const { return _cols; }
};

struct Program {
	Pose2D initPose;
	std::string code;
};

using Procedures = std::map<char, std::string>;
using Programs = std::vector<Program>;
