#include "AStar.h"
#include <queue>
#include <unordered_map>
#include <cmath>
#include <climits>
using namespace std;

// node used in the priority queue
struct Node {
	Position pos;
	int g;  // cost so start
	int h;  // estimated cost to goal

	int f() const { return g + h; }	//total cost
};

// using manhattan distance as heuristic since we only move in 4 directions
int heuristic(Position a, Position b) {
	return abs(a.row - b.row) + abs(a.col - b.col); //distance estimate
}

vector<Position> reconstructPath(unordered_map<int, Position>& cameFrom, Position current) {
	vector<Position> path;
	int key = current.row * Grid::COLS + current.col;		//convert position to unique key

	while (cameFrom.find(key) != cameFrom.end()) {
		path.push_back(current);						//add current step to path
		current = cameFrom[key];						//move to previous position
		key = current.row * Grid::COLS + current.col;	//updated key
	}

	return path;										//return reconstructed path
}

vector<Position> runAStar(const Grid& grid) {
	Position start = grid.getStart();					//get start position
	vector<Position> goals = grid.getGoals();			//get all goal positions
			
	if (start.row == -1 || goals.empty())
		return {};										//stop if no start or no goals

	// min heap - lower f() = higher priority
	auto cmp = [](Node a, Node b) { return a.f() > b.f(); };
	priority_queue<Node, vector<Node>, decltype(cmp)> openList(cmp);

	unordered_map<int, int> gScore;			//best known cost to each cell
	unordered_map<int, Position> cameFrom;	//stores previous step for each cell

	int startKey = start.row * Grid::COLS + start.col;
	gScore[startKey] = 0;					//start cost is 0

	// pick whichever goal is closest as our first h estimate
	int startH = INT_MAX;
	for (auto& g : goals)
		startH = min(startH, heuristic(start, g));	//find closest goal estimate

	openList.push({ start, 0, startH });			//add start node to queue

	// 4 directional movement
	int dr[] = { 1, -1, 0, 0 };
	int dc[] = { 0, 0, 1, -1 };

	while (!openList.empty()) {		
		Node curr = openList.top();			//get best node
		openList.pop();

		// check if we reached any goal
		for (auto& g : goals) {
			if (curr.pos == g)
				return reconstructPath(cameFrom, curr.pos);		//build and return path
		}

		for (int i = 0; i < 4; i++) {
			Position next = { curr.pos.row + dr[i], curr.pos.col + dc[i] };	//next neighbor position

			if (!grid.isWalkable(next.row, next.col))
				continue;			//skip blocked cells

			int nextKey = next.row * Grid::COLS + next.col;
			int newG = curr.g + 1;			//cost to reach next cell

			if (gScore.find(nextKey) == gScore.end() || newG < gScore[nextKey]) {
				cameFrom[nextKey] = curr.pos;		//remember best previous cell
				gScore[nextKey] = newG;				//update best cost

				int h = INT_MAX;
				for (auto& g : goals)
					h = min(h, heuristic(next, g));	//estimate nearest goal

				openList.push({ next, newG, h });	//add neighbor to queue
			}
		}
	}

	return {};  // no path found
}