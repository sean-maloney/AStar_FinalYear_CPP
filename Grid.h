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
	static const int ROWS = 10;
	static const int COLS = 11;

	Grid();

	void loadDefaultLayout();

	void clearAll();
	bool clearCell(const std::string& coord);

	bool setStart(const std::string& coord);
	bool addGoal(const std::string& coord);
	bool toggleObstacle(const std::string& coord);

	void clearPath();
	void markPath(const std::vector<Position>& path);

	void print(bool useColor) const;

	bool isWalkable(int row, int col) const;
	CellType getCell(int row, int col) const;

	Position getStart() const;
	std::vector<Position> getGoals() const;

private:
	std::vector<std::vector<CellType>> cells;
	Position startPos;
	std::vector<Position> goals;

	bool parseCoord(const std::string& coord, Position* pos) const;
	bool inBounds(int row, int col) const;
};