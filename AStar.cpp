#include "AStar.h"
#include <queue>
#include <unordered_map>
#include <cmath>
#include <climits>
using namespace std;

// node used in the priority queue
struct Node {
	Position pos;
	int g;  // cost so far
	int h;  // heuristic

	int f() const { return g + h; }
};

// using manhattan distance as heuristic since we only move in 4 directions
int heuristic(Position a, Position b) {
	return abs(a.row - b.row) + abs(a.col - b.col);
}

vector<Position> reconstructPath(unordered_map<int, Position>& cameFrom, Position current) {
	vector<Position> path;
	int key = current.row * Grid::COLS + current.col;

	while (cameFrom.find(key) != cameFrom.end()) {
		path.push_back(current);
		current = cameFrom[key];
		key = current.row * Grid::COLS + current.col;
	}

	return path;
}

vector<Position> runAStar(const Grid& grid) {
	Position start = grid.getStart();
	vector<Position> goals = grid.getGoals();

	if (start.row == -1 || goals.empty())
		return {};

	// min heap - lower f() = higher priority
	auto cmp = [](Node a, Node b) { return a.f() > b.f(); };
	priority_queue<Node, vector<Node>, decltype(cmp)> openList(cmp);

	unordered_map<int, int> gScore;
	unordered_map<int, Position> cameFrom;

	int startKey = start.row * Grid::COLS + start.col;
	gScore[startKey] = 0;

	// pick whichever goal is closest as our first h estimate
	int startH = INT_MAX;
	for (auto& g : goals)
		startH = min(startH, heuristic(start, g));

	openList.push({ start, 0, startH });

	// 4 directional movement
	int dr[] = { 1, -1, 0, 0 };
	int dc[] = { 0, 0, 1, -1 };

	while (!openList.empty()) {
		Node curr = openList.top();
		openList.pop();

		// check if we reached any goal
		for (auto& g : goals) {
			if (curr.pos == g)
				return reconstructPath(cameFrom, curr.pos);
		}

		for (int i = 0; i < 4; i++) {
			Position next = { curr.pos.row + dr[i], curr.pos.col + dc[i] };

			if (!grid.isWalkable(next.row, next.col))
				continue;

			int nextKey = next.row * Grid::COLS + next.col;
			int newG = curr.g + 1;

			if (gScore.find(nextKey) == gScore.end() || newG < gScore[nextKey]) {
				cameFrom[nextKey] = curr.pos;
				gScore[nextKey] = newG;

				int h = INT_MAX;
				for (auto& g : goals)
					h = min(h, heuristic(next, g));

				openList.push({ next, newG, h });
			}
		}
	}

	return {};  // no path found
}