#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>
#include "AStar.h"
#include "Grid.h"
using namespace std;

string toUpper(string s) {
	for (auto& c : s) c = toupper((unsigned char)c);
	return s;
}

bool parseHeuristic(const string& text, HeuristicType* heuristicType) {
	string value = toUpper(text);

	if (value == "MANHATTAN") {
		*heuristicType = HeuristicType::Manhattan;
		return true;
	}
	if (value == "EUCLIDEAN") {
		*heuristicType = HeuristicType::Euclidean;
		return true;
	}
	if (value == "CHEBYSHEV") {
		*heuristicType = HeuristicType::Chebyshev;
		return true;
	}

	return false;
}

void printHelp() {
	cout << "Commands:\n"
		<< "  START A2\n"
		<< "  GOAL H6\n"
		<< "  OBST C4\n"
		<< "  CLEAR D2\n"
		<< "  CLEARALL\n"
		<< "  RUN\n"
		<< "  HEURISTIC MANHATTAN | EUCLIDEAN | CHEBYSHEV\n"
		<< "  LOAD_DEFAULT\n"
		<< "  COLOR ON | COLOR OFF\n"
		<< "  HELP\n"
		<< "  QUIT\n";
}

int main() {
	Grid grid;
	bool useColor = true;
	HeuristicType currentHeuristic = HeuristicType::Manhattan;

	grid.loadDefaultLayout();
	grid.print(useColor);
	cout << "Current heuristic: " << heuristicToString(currentHeuristic) << "\n";
	printHelp();

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

		if (cmd == "HELP") {
			printHelp();
			continue;
		}

		if (cmd == "START") {
			string coord; iss >> coord;
			if (!grid.setStart(coord))
				cout << "START failed (invalid coord or blocked).\n";
			grid.print(useColor);
			continue;
		}

		if (cmd == "GOAL") {
			string coord; iss >> coord;
			if (!grid.addGoal(coord))
				cout << "GOAL failed (invalid coord or blocked).\n";
			grid.print(useColor);
			continue;
		}

		if (cmd == "OBST") {
			string coord; iss >> coord;
			if (!grid.toggleObstacle(coord))
				cout << "OBST failed (invalid coord or cell is START).\n";
			grid.print(useColor);
			continue;
		}

		if (cmd == "CLEAR") {
			string coord; iss >> coord;
			if (!grid.clearCell(coord))
				cout << "CLEAR failed (invalid coord).\n";
			grid.print(useColor);
			continue;
		}

		if (cmd == "CLEARALL") {
			grid.clearAll();
			grid.print(useColor);
			continue;
		}

		if (cmd == "RUN") {
			grid.clearPath();
			auto path = runAStar(grid, currentHeuristic);
			if (path.empty())
				cout << "No path found.\n";
			else
				grid.markPath(path);
			grid.print(useColor);
			continue;
		}

		if (cmd == "HEURISTIC") {
			string mode; iss >> mode;
			HeuristicType selected;
			if (!parseHeuristic(mode, &selected)) {
				cout << "Invalid heuristic. Use MANHATTAN, EUCLIDEAN, or CHEBYSHEV.\n";
				continue;
			}
			currentHeuristic = selected;
			cout << "Heuristic set to " << heuristicToString(currentHeuristic) << ".\n";
			continue;
		}

		if (cmd == "LOAD_DEFAULT") {
			grid.loadDefaultLayout();
			grid.print(useColor);
			continue;
		}

		if (cmd == "COLOR") {
			string mode; iss >> mode;
			mode = toUpper(mode);
			if (mode == "ON") useColor = true;
			if (mode == "OFF") useColor = false;
			grid.print(useColor);
			continue;
		}

		cout << "Unknown command. Type HELP.\n";
	}

	return 0;
}