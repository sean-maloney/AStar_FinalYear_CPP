#pragma once
#include <vector>

struct Position {
	int row;		//row number
	int col;		//column number

	bool operator==(const Position& other) const {
		return row == other.row && col == other.col; //compare two positions for equality
	}
};