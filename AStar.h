#pragma once
#include <vector>
#include <string>
#include "Grid.h"

enum class HeuristicType {
	Manhattan,
	Euclidean,
	Chebyshev
};

struct SearchStats {
	int nodesExpanded = 0;
	int nodesGenerated = 0;
	int pathLength = 0;
	long long executionMicros = 0;
};

struct SearchResult {
	std::vector<Position> path;
	SearchStats stats;
	Position chosenGoal = { -1, -1 };
	bool foundPath = false;
	bool startWasGoal = false;
};

struct PerformanceRow {
	std::string heuristicName;
	bool foundPath = false;
	int pathLength = 0;
	int nodesExpanded = 0;
	int nodesGenerated = 0;
	long long executionMicros = 0;
	Position chosenGoal = { -1, -1 };
};

SearchResult runAStar(const Grid& grid, HeuristicType heuristicType = HeuristicType::Manhattan);
std::vector<PerformanceRow> runPerformanceOverview(const Grid& grid);
std::string heuristicToString(HeuristicType heuristicType);