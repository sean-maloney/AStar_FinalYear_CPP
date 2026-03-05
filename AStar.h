#pragma once
#include <vector>
#include "Grid.h"

// runs astar on the grid and returns the path
// returns empty vector if no path found
std::vector<Position> runAStar(const Grid& grid);