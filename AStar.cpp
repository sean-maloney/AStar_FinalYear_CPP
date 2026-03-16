#include "AStar.h"
#include <queue>
#include <unordered_map>
#include <cmath>
#include <climits>
#include <algorithm>
using namespace std;

// node used in the priority queue
struct Node {
	Position pos;
	int g;  // cost so far
	int h;  // estimated cost to goal

	int f() const { return g + h; }	// total cost
};

static int positionKey(const Position& pos) {
	return pos.row * Grid::COLS + pos.col;
}

string heuristicToString(HeuristicType heuristicType) {
	switch (heuristicType) {
	case HeuristicType::Manhattan: return "MANHATTAN";
	case HeuristicType::Euclidean: return "EUCLIDEAN";
	case HeuristicType::Chebyshev: return "CHEBYSHEV";
	default: return "MANHATTAN";
	}
}

static bool usesDiagonalMovement(HeuristicType heuristicType) {
	return heuristicType == HeuristicType::Euclidean || heuristicType == HeuristicType::Chebyshev;
}

static int heuristic(Position a, Position b, HeuristicType heuristicType) {
	int rowDiff = abs(a.row - b.row);
	int colDiff = abs(a.col - b.col);

	switch (heuristicType) {
	case HeuristicType::Euclidean:
		return static_cast<int>(round(sqrt(static_cast<double>(rowDiff * rowDiff + colDiff * colDiff)) * 10.0));
	case HeuristicType::Chebyshev:
		return max(rowDiff, colDiff) * 10;
	case HeuristicType::Manhattan:
	default:
		return (rowDiff + colDiff) * 10;
	}
}

static vector<Position> reconstructPath(unordered_map<int, Position>& cameFrom, Position current) {
	vector<Position> path;
	int key = positionKey(current);

	while (cameFrom.find(key) != cameFrom.end()) {
		path.push_back(current);
		current = cameFrom[key];
		key = positionKey(current);
	}

	reverse(path.begin(), path.end());
	return path;
}

static bool canMoveDiagonally(const Grid& grid, Position current, Position next) {
	int rowStep = next.row - current.row;
	int colStep = next.col - current.col;

	if (abs(rowStep) != 1 || abs(colStep) != 1)
		return true;

	int sideRow1 = current.row + rowStep;
	int sideCol1 = current.col;
	int sideRow2 = current.row;
	int sideCol2 = current.col + colStep;

	return grid.isWalkable(sideRow1, sideCol1) && grid.isWalkable(sideRow2, sideCol2);
}

static int moveCost(Position current, Position next, HeuristicType heuristicType) {
	bool isDiagonal = (current.row != next.row) && (current.col != next.col);

	if (!isDiagonal)
		return 10;

	if (heuristicType == HeuristicType::Chebyshev)
		return 10;

	return 14;
}

vector<Position> runAStar(const Grid& grid, HeuristicType heuristicType) {
	Position start = grid.getStart();
	vector<Position> goals = grid.getGoals();

	if (start.row == -1 || goals.empty())
		return {};

	auto cmp = [](Node a, Node b) { return a.f() > b.f(); };
	priority_queue<Node, vector<Node>, decltype(cmp)> openList(cmp);

	unordered_map<int, int> gScore;
	unordered_map<int, Position> cameFrom;

	int startKey = positionKey(start);
	gScore[startKey] = 0;

	int startH = INT_MAX;
	for (auto& g : goals)
		startH = min(startH, heuristic(start, g, heuristicType));

	openList.push({ start, 0, startH });

	int dr4[] = { 1, -1, 0, 0 };
	int dc4[] = { 0, 0, 1, -1 };

	int dr8[] = { 1, -1, 0, 0, 1, 1, -1, -1 };
	int dc8[] = { 0, 0, 1, -1, 1, -1, 1, -1 };

	bool diagonalAllowed = usesDiagonalMovement(heuristicType);

	while (!openList.empty()) {
		Node curr = openList.top();
		openList.pop();

		for (auto& g : goals) {
			if (curr.pos == g)
				return reconstructPath(cameFrom, curr.pos);
		}

		int moveCount = diagonalAllowed ? 8 : 4;

		for (int i = 0; i < moveCount; i++) {
			Position next;

			if (diagonalAllowed)
				next = { curr.pos.row + dr8[i], curr.pos.col + dc8[i] };
			else
				next = { curr.pos.row + dr4[i], curr.pos.col + dc4[i] };

			if (!grid.isWalkable(next.row, next.col))
				continue;

			if (diagonalAllowed && !canMoveDiagonally(grid, curr.pos, next))
				continue;

			int nextKey = positionKey(next);
			int newG = curr.g + moveCost(curr.pos, next, heuristicType);

			if (gScore.find(nextKey) == gScore.end() || newG < gScore[nextKey]) {
				cameFrom[nextKey] = curr.pos;
				gScore[nextKey] = newG;

				int h = INT_MAX;
				for (auto& g : goals)
					h = min(h, heuristic(next, g, heuristicType));

				openList.push({ next, newG, h });
			}
		}
	}

	return {};
}