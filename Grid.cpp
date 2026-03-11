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
	cells.resize(ROWS, vector<CellType>(COLS, CellType::Empty));	//create empty grid
	startPos = { -1, -1 };											//no start position yet
}

void Grid::loadDefaultLayout() {
	// reset everything first
	for (int r = 0; r < ROWS; r++)
		for (int c = 0; c < COLS; c++)
			cells[r][c] = CellType::Empty;
	goals.clear();					//remove old goals
	startPos = { -1, -1 };			//remove old start

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
		cells[p.row][p.col] = CellType::Goal;	//place goal on grid
		goals.push_back(p);						//store goal position
	}
}

void Grid::clearAll() {
	for (int r = 0; r < ROWS; r++)
		for (int c = 0; c < COLS; c++)
			cells[r][c] = CellType::Empty;		//clear every cell
	goals.clear();								//remove all goals
	startPos = { -1, -1 };						//remove start
}

bool Grid::clearCell(const string& coord) {
	Position pos;
	if (!parseCoord(coord, &pos)) return false;//incalid cordinate

	if (cells[pos.row][pos.col] == CellType::Start)
		startPos = { -1, -1 };					//remove saved start position

	if (cells[pos.row][pos.col] == CellType::Goal) {
		for (int i = 0; i < (int)goals.size(); i++) {
			if (goals[i] == pos) {
				goals.erase(goals.begin() + i);	//remove goal from list
				break;
			}
		}
	}

	cells[pos.row][pos.col] = CellType::Empty;		//make cell empty
	return true;
}

bool Grid::setStart(const string& coord) {
	Position pos;
	if (!parseCoord(coord, &pos)) return false;		//invalid cordinate
	if (!isWalkable(pos.row, pos.col)) return false;//can't place on obstacle

	if (cells[pos.row][pos.col] == CellType::Goal) {
		for (int i = 0; i < (int)goals.size(); i++) {
			if (goals[i] == pos) {
				goals.erase(goals.begin() + i);		//remove goal if replacing it
				break;
			}
		}
	}

	if (startPos.row != -1)
		cells[startPos.row][startPos.col] = CellType::Empty;	//clear old start

	startPos = pos;												//save new start position
	cells[pos.row][pos.col] = CellType::Start;					//mark start on grid
	return true;
}

bool Grid::addGoal(const string& coord) {
	Position pos;
	if (!parseCoord(coord, &pos)) return false;					//inalid cordinate
	if (!isWalkable(pos.row, pos.col)) return false;			//cannot place on obstacle

	cells[pos.row][pos.col] = CellType::Goal;					//mark goal on grid
	goals.push_back(pos);										//save goal position
	return true;
}

bool Grid::toggleObstacle(const string& coord) {
	Position pos;
	if (!parseCoord(coord, &pos)) return false;					//invalid cordinate

	CellType cell = cells[pos.row][pos.col];					//get current cell type

	if (cell == CellType::Start) return false;					//cannoy block the start

	if (cell == CellType::Goal) {			
		for (int i = 0; i < (int)goals.size(); i++) {
			if (goals[i] == pos) {
				goals.erase(goals.begin() + i);					//remove goal from list
				break;
			}
		}
		cells[pos.row][pos.col] = CellType::Obstacle;			//turn goal into coordinate
		return true;
	}

	if (cell == CellType::Obstacle)
		cells[pos.row][pos.col] = CellType::Empty;				//renive obstacle
	else
		cells[pos.row][pos.col] = CellType::Obstacle;			//place obstacle

	return true;
}

void Grid::clearPath() {
	for (int r = 0; r < ROWS; r++)
		for (int c = 0; c < COLS; c++)
			if (cells[r][c] == CellType::Path)
				cells[r][c] = CellType::Empty;					//remove old path only
}

void Grid::markPath(const vector<Position>& path) {
	for (int i = 0; i < (int)path.size(); i++) {
		if (cells[path[i].row][path[i].col] == CellType::Empty)
			cells[path[i].row][path[i].col] = CellType::Path;	//draw pah on cempty cells
	}
}

void Grid::print(bool useColor) const {
	cout << "   ";
	for (int c = 0; c < COLS; c++)
		cout << (char)('A' + c) << " ";							//print column letters
	cout << endl;

	for (int r = 0; r < ROWS; r++) {
		cout << r << "  ";										//print row number
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
				cout << col << sym << RESET << " ";				//print colored symbol
			else
				cout << sym << " ";								//print normal symbol
		}
		cout << endl;
	}
}

bool Grid::isWalkable(int row, int col) const {
	if (!inBounds(row, col)) return false;						//outside grid
	return cells[row][col] != CellType::Obstacle;				//driveable if now blocked
}

CellType Grid::getCell(int row, int col) const { return cells[row][col]; }	//return cell type
Position Grid::getStart() const { return startPos; }						//return start position
vector<Position> Grid::getGoals() const { return goals; }					//return all goals

bool Grid::parseCoord(const string& coord, Position* pos) const {
	if (coord.size() < 2) return false;									//must conatin letter and number
	if (!isalpha((unsigned char)coord[0])) return false;				//first character must be a letter

	int col = toupper(coord[0]) - 'A';						//convert letter to column number
	int row = 0;
	try {
		row = stoi(coord.substr(1));						//convert rest to row number
	}
	catch (...) {
		return false;		//invalid row number
	}

	if (!inBounds(row, col)) return false;		//coordinate must be within grid
	pos->row = row;
	pos->col = col;
	return true;
}

bool Grid::inBounds(int row, int col) const {
	return row >= 0 && row < ROWS && col >= 0 && col < COLS;  //check calid gri range
}