#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>
#include "AStar.h"
#include "Grid.h"
using namespace std;

string toUpper(string s) {
	for (auto& c : s) c = toupper(c);
	return s;
}

int main() {
	Grid grid;
	grid.print(false);

	while (true) {
		cout << "\nCommand: ";
		string line;
		if (!getline(cin, line)) break;

		istringstream iss(line);
		string cmd;
		iss >> cmd;
		cmd = toUpper(cmd);

		if (cmd.empty()) continue;
		if (cmd == "QUIT") break;

		if (cmd == "START") {
			string coord; iss >> coord;
			if (!grid.setStart(coord))
				cout << "START failed (invalid coord or blocked).\n";
			grid.print(false);
			continue;
		}

		if (cmd == "GOAL") {
			string coord; iss >> coord;
			if (!grid.addGoal(coord))
				cout << "GOAL failed (invalid coord or blocked).\n";
			grid.print(false);
			continue;
		}

		if (cmd == "OBST") {
			string coord; iss >> coord;
			if (!grid.toggleObstacle(coord))
				cout << "OBST failed (invalid coord or cell is START).\n";
			grid.print(false);
			continue;
		}

		if (cmd == "CLEAR") {
			string coord; iss >> coord;
			if (!grid.clearCell(coord))
				cout << "CLEAR failed (invalid coord).\n";
			grid.print(false);
			continue;
		}

		if (cmd == "CLEARALL") {
			grid.clearAll();
			grid.print(false);
			continue;
		}

		if (cmd == "RUN") {
			grid.clearPath();
			auto path = runAStar(grid);
			if (path.empty())
				cout << "No path found.\n";
			else
				grid.markPath(path);
			grid.print(false);
			continue;
		}

		cout << "Unknown command.\n";
	}

	return 0;
}