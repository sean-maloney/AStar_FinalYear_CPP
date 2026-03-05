#include "Grid.h"
#include <iostream>
using namespace std;

Grid::Grid() {
	cells.resize(ROWS, vector<CellType>(COLS, CellType::Empty));
	startPos = { -1, -1 };
}

// TODO: implement these properly
void Grid::clearAll() {}
bool Grid::clearCell(const string& coord) { return false; }
bool Grid::setStart(const string& coord) { return false; }
bool Grid::addGoal(const string& coord) { return false; }
bool Grid::toggleObstacle(const string& coord) { return false; }
void Grid::clearPath() {}
void Grid::markPath(const vector<Position>& path) {}

void Grid::print(bool useColor) const {
	// print column headers
	cout << "   ";
	for (int c = 0; c < COLS; c++)
		cout << (char)('A' + c) << " ";
	cout << endl;

	for (int r = 0; r < ROWS; r++) {
		cout << r << "  ";
		for (int c = 0; c < COLS; c++)
			cout << ". ";
		cout << endl;
	}
}

bool Grid::isWalkable(int row, int col) const {
	if (!inBounds(row, col)) return false;
	return cells[row][col] != CellType::Obstacle;
}

CellType Grid::getCell(int row, int col) const {
	return cells[row][col];
}

Position Grid::getStart() const { return startPos; }
vector<Position> Grid::getGoals() const { return goals; }

bool Grid::parseCoord(const string& coord, Position* pos) const {
	if (coord.size() < 2) return false;
	int col = toupper(coord[0]) - 'A';
	int row = stoi(coord.substr(1));
	if (!inBounds(row, col)) return false;
	pos->row = row;
	pos->col = col;
	return true;
}

bool Grid::inBounds(int row, int col) const {
	return row >= 0 && row < ROWS && col >= 0 && col < COLS;
}