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

		cout << "Unknown command.\n";
	}

	return 0;
}