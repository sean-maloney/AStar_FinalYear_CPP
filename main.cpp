#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>
#include "AStar.h"
#include "Grid.h"
using namespace std;

string toUpper(string s) {
	for (auto& c : s) c = toupper(c); //convert each character to uppercase
	return s;						  // return updated string
}

void printHelp() {
	cout << "Commands:\n"
		<< "  START A2\n"
		<< "  GOAL H6\n"
		<< "  OBST C4\n"
		<< "  CLEAR D2\n"
		<< "  CLEARALL\n"
		<< "  RUN\n"
		<< "  LOAD_DEFAULT\n"
		<< "  COLOR ON | COLOR OFF\n"
		<< "  HELP\n"
		<< "  QUIT\n";
}

int main() {
	Grid grid;
	bool useColor = true;

	grid.loadDefaultLayout();
	grid.print(useColor);
	printHelp();

	while (true) {	//keep running until user quits
		cout << "\nCommand: ";
		string line;
		if (!getline(cin, line)) break; //stop if input fails

		istringstream iss(line);		//read cinnabd from full input line
		string cmd;
		iss >> cmd;
		cmd = toUpper(cmd);				//calls make uppercase command

		if (cmd.empty()) continue;		//ignore empty input
		if (cmd == "QUIT") break;		//exit program

		if (cmd == "HELP") {
			printHelp();				//show list of commands
			continue;
		}

		if (cmd == "START") {
			string coord; iss >> coord;			//read coorfinate after START
			if (!grid.setStart(coord))
				cout << "START failed (invalid coord or blocked).\n";
			grid.print(useColor);				//prints updated grid
			continue;
		}

		if (cmd == "GOAL") {
			string coord; iss >> coord;			//read coorfinate after GOAL
			if (!grid.addGoal(coord))
				cout << "GOAL failed (invalid coord or blocked).\n";
			grid.print(useColor);				//prints updated grid
			continue;
		}

		if (cmd == "OBST") {
			string coord; iss >> coord;			//read coorfinate after OBST
			if (!grid.toggleObstacle(coord))
				cout << "OBST failed (invalid coord or cell is START).\n";
			grid.print(useColor);				//prints updated grid
			continue;
		}

		if (cmd == "CLEAR") {
			string coord; iss >> coord;			//read coorfinate after CLEAR
			if (!grid.clearCell(coord))
				cout << "CLEAR failed (invalid coord).\n";
			grid.print(useColor);				//prints updated grid
			continue;
		}

		if (cmd == "CLEARALL") {
			grid.clearAll();					//Clears entire grid
			grid.print(useColor);
			continue;
		}

		if (cmd == "RUN") {
			grid.clearPath();					//Remove old path before finding new one
			auto path = runAStar(grid);			//Runs A* pathfinding
			if (path.empty())
				cout << "No path found.\n";
			else
				grid.markPath(path);			//draw found path on grid
			grid.print(useColor);
			continue;
		}

		if (cmd == "LOAD_DEFAULT") {
			grid.loadDefaultLayout();			//load preset map layout
			grid.print(useColor);
			continue;
		}

		if (cmd == "COLOR") {					//Toggles color mode on or off
			string mode; iss >> mode;
			mode = toUpper(mode);
			if (mode == "ON")  useColor = true;
			if (mode == "OFF") useColor = false;
			grid.print(useColor);
			continue;
		}

		cout << "Unknown command. Type HELP.\n";	//message for invalid command
	}

	return 0;
}