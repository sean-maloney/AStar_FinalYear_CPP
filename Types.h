#pragma once
#include <vector>
#include <cstdint>

enum class CellType : uint8_t { Empty = 0, Obstacle = 1, Goal = 2 };

struct IVec2 {
    int x = 0, y = 0;
    bool operator==(const IVec2& o) const { return x == o.x && y == o.y; }
};

struct PathResult {
    bool found = false;
    std::vector<IVec2> path;
};
