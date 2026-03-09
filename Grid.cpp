#include "Grid.h"
#include <iostream>
#include <algorithm>
#include <cctype>
#include <stdexcept>
using namespace std;

#define RED    "\033[31m"
#define GREEN  "\033[32m"
#define BLUE   "\033[34m"
#define YELLOW "\033[33m"
#define RESET  "\033[0m"

Grid::Grid() {
	cells.resize(ROWS, vector<CellType>(COLS, CellType::Empty));
	startPos = { -1, -1 };
}

void Grid::loadDefaultLayout() {
	// reset everything first
	for (int r = 0; r < ROWS; r++)
		for (int c = 0; c < COLS; c++)
			cells[r][c] = CellType::Empty;
	goals.clear();
	startPos = { -1, -1 };

	// helper to fill a rectangle with obstacles
	auto fillRect = [&](int r0, int c0, int r1, int c1) {
		for (int r = r0; r <= r1; r++)
			for (int c = c0; c <= c1; c++)
				cells[r][c] = CellType::Obstacle;
		};

	fillRect(1, 1, 1, 9);   // top row of parked cars
	fillRect(4, 1, 5, 9);   // middle double row
	fillRect(8, 1, 8, 10);  // bottom row

	// open parking spots
	Position spots[] = {
		{1, 3}, {1, 7},
		{4, 6}, {5, 6},
		{8, 2}, {8, 9},
	};

	for (auto& p : spots) {
		cells[p.row][p.col] = CellType::Goal;
		goals.push_back(p);
	}
}

void Grid::clearAll() {
	for (int r = 0; r < ROWS; r++)
		for (int c = 0; c < COLS; c++)
			cells[r][c] = CellType::Empty;
	goals.clear();
	startPos = { -1, -1 };
}

bool Grid::clearCell(const string& coord) {
	Position pos;
	if (!parseCoord(coord, &pos)) return false;

	if (cells[pos.row][pos.col] == CellType::Start)
		startPos = { -1, -1 };

	if (cells[pos.row][pos.col] == CellType::Goal) {
		for (int i = 0; i < (int)goals.size(); i++) {
			if (goals[i] == pos) {
				goals.erase(goals.begin() + i);
				break;
			}
		}
	}

	cells[pos.row][pos.col] = CellType::Empty;
	return true;
}

bool Grid::setStart(const string& coord) {
	Position pos;
	if (!parseCoord(coord, &pos)) return false;
	if (!isWalkable(pos.row, pos.col)) return false;

	if (cells[pos.row][pos.col] == CellType::Goal) {
		for (int i = 0; i < (int)goals.size(); i++) {
			if (goals[i] == pos) {
				goals.erase(goals.begin() + i);
				break;
			}
		}
	}

	if (startPos.row != -1)
		cells[startPos.row][startPos.col] = CellType::Empty;

	startPos = pos;
	cells[pos.row][pos.col] = CellType::Start;
	return true;
}

bool Grid::addGoal(const string& coord) {
	Position pos;
	if (!parseCoord(coord, &pos)) return false;
	if (!isWalkable(pos.row, pos.col)) return false;

	cells[pos.row][pos.col] = CellType::Goal;
	goals.push_back(pos);
	return true;
}

bool Grid::toggleObstacle(const string& coord) {
	Position pos;
	if (!parseCoord(coord, &pos)) return false;

	CellType cell = cells[pos.row][pos.col];

	if (cell == CellType::Start) return false;

	if (cell == CellType::Goal) {
		for (int i = 0; i < (int)goals.size(); i++) {
			if (goals[i] == pos) {
				goals.erase(goals.begin() + i);
				break;
			}
		}
		cells[pos.row][pos.col] = CellType::Obstacle;
		return true;
	}

	if (cell == CellType::Obstacle)
		cells[pos.row][pos.col] = CellType::Empty;
	else
		cells[pos.row][pos.col] = CellType::Obstacle;

	return true;
}

void Grid::clearPath() {
	for (int r = 0; r < ROWS; r++)
		for (int c = 0; c < COLS; c++)
			if (cells[r][c] == CellType::Path)
				cells[r][c] = CellType::Empty;
}

void Grid::markPath(const vector<Position>& path) {
	for (int i = 0; i < (int)path.size(); i++) {
		if (cells[path[i].row][path[i].col] == CellType::Empty)
			cells[path[i].row][path[i].col] = CellType::Path;
	}
}

void Grid::print(bool useColor) const {
	cout << "   ";
	for (int c = 0; c < COLS; c++)
		cout << (char)('A' + c) << " ";
	cout << endl;

	for (int r = 0; r < ROWS; r++) {
		cout << r << "  ";
		for (int c = 0; c < COLS; c++) {
			char sym = '.';
			const char* col = "";

			switch (cells[r][c]) {
			case CellType::Obstacle: sym = 'X'; col = RED;    break;
			case CellType::Start:    sym = 'S'; col = BLUE;   break;
			case CellType::Goal:     sym = 'G'; col = GREEN;  break;
			case CellType::Path:     sym = '*'; col = YELLOW; break;
			default: break;
			}

			if (useColor && cells[r][c] != CellType::Empty)
				cout << col << sym << RESET << " ";
			else
				cout << sym << " ";
		}
		cout << endl;
	}
}

bool Grid::isWalkable(int row, int col) const {
	if (!inBounds(row, col)) return false;
	return cells[row][col] != CellType::Obstacle;
}

CellType Grid::getCell(int row, int col) const { return cells[row][col]; }
Position Grid::getStart() const { return startPos; }
vector<Position> Grid::getGoals() const { return goals; }

bool Grid::parseCoord(const string& coord, Position* pos) const {
	if (coord.size() < 2) return false;
	if (!isalpha((unsigned char)coord[0])) return false;

	int col = toupper(coord[0]) - 'A';
	int row = 0;
	try {
		row = stoi(coord.substr(1));
	}
	catch (...) {
		return false;
	}

	if (!inBounds(row, col)) return false;
	pos->row = row;
	pos->col = col;
	return true;
}

bool Grid::inBounds(int row, int col) const {
	return row >= 0 && row < ROWS && col >= 0 && col < COLS;
}