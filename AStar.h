#pragma once
#include <vector>
#include <string>
#include "Grid.h"

enum class HeuristicType {
	Manhattan,
	Euclidean,
	Chebyshev
};

std::vector<Position> runAStar(const Grid& grid, HeuristicType heuristicType = HeuristicType::Manhattan);
std::string heuristicToString(HeuristicType heuristicType);