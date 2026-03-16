#include "AStar.h"
#include <queue>
#include <unordered_map>
#include <cmath>
#include <climits>
#include <chrono>
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

static int heuristic(Position a, Position b, HeuristicType heuristicType) {
	int rowDiff = abs(a.row - b.row);
	int colDiff = abs(a.col - b.col);

	switch (heuristicType) {
	case HeuristicType::Euclidean:
		return static_cast<int>(round(sqrt(static_cast<double>(rowDiff * rowDiff + colDiff * colDiff))));
	case HeuristicType::Chebyshev:
		return max(rowDiff, colDiff);
	case HeuristicType::Manhattan:
	default:
		return rowDiff + colDiff;
	}
}

static bool isGoal(const Position& pos, const vector<Position>& goals) {
	for (const auto& goal : goals) {
		if (pos == goal)
			return true;
	}
	return false;
}

static int getClosestGoalEstimate(Position current, const vector<Position>& goals, HeuristicType heuristicType) {
	int bestDistance = INT_MAX;

	for (const auto& goal : goals)
		bestDistance = min(bestDistance, heuristic(current, goal, heuristicType));

	return bestDistance;
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

SearchResult runAStar(const Grid& grid, HeuristicType heuristicType) {
	SearchResult result;
	auto startTime = chrono::high_resolution_clock::now();

	Position start = grid.getStart();
	vector<Position> goals = grid.getGoals();

	if (start.row == -1 || goals.empty()) {
		auto endTime = chrono::high_resolution_clock::now();
		result.stats.executionMicros =
			chrono::duration_cast<chrono::microseconds>(endTime - startTime).count();
		return result;
	}

	if (isGoal(start, goals)) {
		result.foundPath = true;
		result.startWasGoal = true;
		result.chosenGoal = start;
		auto endTime = chrono::high_resolution_clock::now();
		result.stats.executionMicros =
			chrono::duration_cast<chrono::microseconds>(endTime - startTime).count();
		return result;
	}

	auto cmp = [](Node a, Node b) { return a.f() > b.f(); };
	priority_queue<Node, vector<Node>, decltype(cmp)> openList(cmp);

	unordered_map<int, int> gScore;
	unordered_map<int, Position> cameFrom;

	int startKey = positionKey(start);
	gScore[startKey] = 0;

	int startH = getClosestGoalEstimate(start, goals, heuristicType);
	openList.push({ start, 0, startH });
	result.stats.nodesGenerated = 1;

	int dr[] = { 1, -1, 0, 0 };
	int dc[] = { 0, 0, 1, -1 };

	while (!openList.empty()) {
		Node curr = openList.top();
		openList.pop();
		result.stats.nodesExpanded++;

		if (isGoal(curr.pos, goals)) {
			result.path = reconstructPath(cameFrom, curr.pos);
			result.stats.pathLength = static_cast<int>(result.path.size());
			result.chosenGoal = curr.pos;
			result.foundPath = true;

			auto endTime = chrono::high_resolution_clock::now();
			result.stats.executionMicros =
				chrono::duration_cast<chrono::microseconds>(endTime - startTime).count();
			return result;
		}

		for (int i = 0; i < 4; i++) {
			Position next = { curr.pos.row + dr[i], curr.pos.col + dc[i] };

			if (!grid.isWalkable(next.row, next.col))
				continue;

			int nextKey = positionKey(next);
			int newG = curr.g + 1;

			if (gScore.find(nextKey) == gScore.end() || newG < gScore[nextKey]) {
				cameFrom[nextKey] = curr.pos;
				gScore[nextKey] = newG;

				int h = getClosestGoalEstimate(next, goals, heuristicType);
				openList.push({ next, newG, h });
				result.stats.nodesGenerated++;
			}
		}
	}

	auto endTime = chrono::high_resolution_clock::now();
	result.stats.executionMicros =
		chrono::duration_cast<chrono::microseconds>(endTime - startTime).count();
	return result;
}

vector<PerformanceRow> runPerformanceOverview(const Grid& grid) {
	vector<PerformanceRow> rows;
	HeuristicType heuristics[] = {
		HeuristicType::Manhattan,
		HeuristicType::Euclidean,
		HeuristicType::Chebyshev
	};

	for (HeuristicType heuristicType : heuristics) {
		SearchResult result = runAStar(grid, heuristicType);

		PerformanceRow row;
		row.heuristicName = heuristicToString(heuristicType);
		row.foundPath = result.foundPath;
		row.pathLength = result.stats.pathLength;
		row.nodesExpanded = result.stats.nodesExpanded;
		row.nodesGenerated = result.stats.nodesGenerated;
		row.executionMicros = result.stats.executionMicros;
		row.chosenGoal = result.chosenGoal;
		rows.push_back(row);
	}

	return rows;
}