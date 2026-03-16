#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <iomanip>
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
		<< "  REVIEW\n"
		<< "  LOAD_DEFAULT\n"
		<< "  COLOR ON | COLOR OFF\n"
		<< "  HELP\n"
		<< "  QUIT\n";
}

void printRunSummary(const SearchResult& result, HeuristicType heuristicType) {
	cout << "Heuristic used: " << heuristicToString(heuristicType) << "\n";
	cout << "Nodes expanded: " << result.stats.nodesExpanded << "\n";
	cout << "Nodes generated: " << result.stats.nodesGenerated << "\n";
	cout << "Execution time: " << result.stats.executionMicros << " microseconds\n";

	if (result.foundPath) {
		cout << "Chosen goal: " << char('A' + result.chosenGoal.col) << result.chosenGoal.row << "\n";
		cout << "Path length: " << result.stats.pathLength << "\n";
		if (result.startWasGoal)
			cout << "Start position is already on a goal cell.\n";
	}
}

void printPerformanceOverview(const vector<PerformanceRow>& rows) {
	cout << "\nPerformance overview\n";
	cout << left
		<< setw(12) << "Heuristic"
		<< setw(8) << "Found"
		<< setw(12) << "Goal"
		<< setw(12) << "PathLen"
		<< setw(12) << "Expanded"
		<< setw(12) << "Generated"
		<< setw(14) << "Time(us)"
		<< "\n";

	cout << string(82, '-') << "\n";

	for (const auto& row : rows) {
		string goalText = "-";
		if (row.chosenGoal.row != -1)
			goalText = string(1, char('A' + row.chosenGoal.col)) + to_string(row.chosenGoal.row);

		cout << left
			<< setw(12) << row.heuristicName
			<< setw(8) << (row.foundPath ? "Yes" : "No")
			<< setw(12) << goalText
			<< setw(12) << row.pathLength
			<< setw(12) << row.nodesExpanded
			<< setw(12) << row.nodesGenerated
			<< setw(14) << row.executionMicros
			<< "\n";
	}
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
			SearchResult result = runAStar(grid, currentHeuristic);
			if (!result.foundPath)
				cout << "No path found.\n";
			else
				grid.markPath(result.path);
			grid.print(useColor);
			printRunSummary(result, currentHeuristic);
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

		if (cmd == "REVIEW") {
			vector<PerformanceRow> rows = runPerformanceOverview(grid);
			printPerformanceOverview(rows);
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
			if (mode == "ON")  useColor = true;
			if (mode == "OFF") useColor = false;
			grid.print(useColor);
			continue;
		}

		cout << "Unknown command. Type HELP.\n";
	}

	return 0;
}