#pragma once
#include <string>
#include <vector>
#include "Types.h"

enum class CellType {
	Empty,
	Obstacle,
	Start,
	Goal,
	Path
};

class Grid {
public:
	static const int ROWS = 10;		//number of rows
	static const int COLS = 11;		//number of columns

	Grid();							//constructor 

	void loadDefaultLayout();		//loads premade grid layout

	void clearAll();				//clear entire grid
	bool clearCell(const std::string& coord);	//clear one cell

	bool setStart(const std::string& coord);	//place start position
	bool addGoal(const std::string& coord);
	bool toggleObstacle(const std::string& coord);

	void clearPath();
	void markPath(const std::vector<Position>& path); //draw path on grid

	void print(bool useColor) const;		//display grid

	bool isWalkable(int row, int col) const;	//check if cell can be used
	CellType getCell(int row, int col) const;	//checks cell type

	Position getStart() const;					//return start position
	std::vector<Position> getGoals() const;

private:
	std::vector<std::vector<CellType>> cells;	//grid cells
	Position startPos;							//curent start
	std::vector<Position> goals;				//all goals

	bool parseCoord(const std::string& coord, Position* pos) const;	//text to position
	bool inBounds(int row, int col) const;				//check valid grid range
};