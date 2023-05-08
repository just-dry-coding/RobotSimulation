#pragma once

#include "Datatypes.h"

#include <istream>
#include <iterator>

class InputParser
{
	Grid2D _grid;
	Procedures _procedures;
	Programs _programs;

public:

	struct MetaInfo
	{
		unsigned int gridRows;
		unsigned int gridCols;
		unsigned int nProcedures;
		unsigned int nPrograms;
	};

	using LinesIterator = std::vector<std::string>::const_iterator;

	InputParser(std::istream& input);

	static std::vector<std::string> getInputLines(std::istream& input);

	static MetaInfo parseFirstLine(std::string const& line);

	static Grid2D parseGrid2D(unsigned lineRows, unsigned lineCols, LinesIterator gridBegin);

	static Procedures parseProcedures(unsigned nProcedures, LinesIterator proceduresBegin);

	static Programs parsePrograms(unsigned nPrograms, LinesIterator programsBegin);

	Grid2D const& grid() const { return _grid; }

	Procedures const& procedures() const { return _procedures; }

	Programs const& programs() const { return _programs; }
};
