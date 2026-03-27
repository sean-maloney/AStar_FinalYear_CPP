---
layout: page
title: A* Pathfinding Layer for Smart Parking Car Park
---

# **WELCOME TO MY A* PATHFINDING PROJECT**

Hi my name is Seán Maloney and this is my A* Pathfinding Project. This project was developed for my C++ module, where the task was to implement the A* algorithm in modern C++. Although the algorithm itself was required I decided, I wanted to make the project more personal by designing it in a way that it supports my final year project, which is based on a Smart Car Parking Solution.

In my final year project, the overall goal is to detect parking spaces and identify available spots in a car park. This A* project is intended to act as the route finding layer on top of that system. Once the parking spaces and blocked areas are known, this part of the project can be used to calculate the closest valid route from a vehicle’s starting position to the closest free parking space.

## **Project Overview**

The main purpose of this project was to build a working implementation of the A* algorithm in C++ and apply it to a grid-based car park simulation. The grid contains:

- a start position
- one or more goal positions
- blocked cells
- empty cells

I adapted the project to fit a parking scenario. In this version:

- the **start node** represents the vehicle or entrance point
- the **goal nodes** represent available parking spaces
- the **obstacles** represent parked cars, blocked zones, or inaccessible parts of the car park

This made this project easier to connect to my final year project.

The main aims of the project were:

- To implement the A* algorithm correctly in modern C++
- To represent a grid based simulation with obstacles
- To calculate and display the shortest valid route
- To keep the program modular and maintainable in case of future use
- To experiment with different heuristics and compare their behaviour
- To build something that can later support the Smart Parking Car Park project

## **How This Fits Into My Final Year Project**

This project is intended to be used as a layer in my final year project rather than being completely separate from it. The wider Smart Parking Car Park idea is focused on identifying cars and parking spaces, then using that information to help guide a driver to a free parking space.

The way I see the full pipeline is:

1. the system detects the parking layout and available spaces  
2. that information is converted into a grid or map  
3. this A* layer takes the vehicle position and the available spaces  
4. it calculates the shortest valid route to the most suitable free space  
5. the final route can then be shown visually as guidance  

So this project is about creating a useful navigation layer that could sit on top of the smart parking detection system.

<img src="assets/images/car-park-layout.png" alt="Car park layout used as the basis for the pathfinding layer">

## **How the Program Works**

The program uses a 2D grid made up of cells. Some cells are empty and can be moved through, while others are obstacles and block movement. A start point is placed on the grid, and one or more goals can also be added.

The A* algorithm works by combining three values:

- **g-cost** which is the movement cost from the start node to the current node
- **h-cost** which is the estimated cost from the current node to the goal
- **f-cost** which is the total cost, calculated as `f = g + h`

The algorithm repeatedly explores the node with the lowest estimated total cost until it either reaches a valid goal or determines that no route exists.

What makes this project more relevant to the Smart Car Parking Solution is that the goal is not just one fixed endpoint. Instead, the algorithm can work with multiple possible parking spaces and find a valid route to one of them.

## **Program Structure**

One of the first things I did was set up the file structure properly. I knew if I rushed straight into the algorithm and kept everything in one file, it would get messy quickly and be harder to debug later.

The project is split into the following files:

- `Types.h`
- `Grid.h`
- `Grid.cpp`
- `AStar.h`
- `AStar.cpp`
- `main.cpp`

I built it this way so each part of the project would have a clear role.

### **Types**
The `Types.h` file contains the shared structures and enums used throughout the project. One of the first things I added here was the `Position` struct, because row and column values are used all across the program.

### **Grid**
The `Grid` class is responsible for managing the environment. It handles:

- the start position
- the goal positions
- obstacle placement
- clearing and resetting cells
- marking the final path
- printing the grid

This was useful because it kept the car park layout logic separate from the actual pathfinding.

### **AStar**
The `AStar` module contains the algorithm itself. This is where the search happens. It handles:

- exploring neighbouring cells
- calculating movement costs
- applying the heuristic
- storing the parent nodes
- reconstructing the path once a goal is reached

This is the core part of the project and the part I spent most time thinking through carefully.

### **Main**
The `main.cpp` file handles the terminal commands and user interaction. It allows the user to:

- set the start point
- add goals
- place or remove obstacles
- clear cells
- run the algorithm
- change heuristics
- review performance results

This command-based setup is really useful because I could test the project much faster without having to keep changing the code itself.

## **Use of A* in This Project**

A* in my project suits this kind of problem well because it is designed for shortest-path searching in grid-based spaces and uses a heuristic to guide the search efficiently. That made it a good fit for modelling a car park layout with blocked areas, a vehicle position, and several possible parking spaces.

The most important design decisions in my project were:

- how to represent the parking environment
- how to structure the program cleanly
- how to support multiple parking goals
- how to test different heuristics
- how to make the project useful for my final year system

That is where most of the individuality of the project comes from.

## **Why I Used a Car Park Layout**

I wanted this project to feel connected to my final year project from the start. That is why I decided to model it as a car park rather than a plain generic grid.

This was important because:

- a parking system naturally has multiple possible goal positions
- obstacles make sense as blocked bays, parked cars, or no-drive zones
- the pathfinding becomes more realistic
- the project feels like a real system component instead of just an exercise

The multiple goal idea was especially important because in a real car park the user usually does not need one exact parking spot. They need the closest one. That made the A* implementation feel much more relevant.

## **Development Process and Thought Process**

I developed the project in stages instead of trying to jump straight into the finished version. That made it easier to understand what I was building and also helped me debug problems as they appeared. (Need to expand Maybe)

### **Starting with the file structure**
When I first properly began the project, I focused on setting up the file structure. I created `Types.h`, `Grid.h`, `Grid.cpp`, `AStar.h`, `AStar.cpp`, and `main.cpp` so I would have a proper base to work from.

At this stage, most of the methods were just placeholders. The grid functions were stubs, `runAStar()` just returned an empty vector, and the command loop in `main.cpp` could only read input and exit on `QUIT`. Even though it did not really do anything yet, it gave me a strong structure to build on.

### **Making the grid actually work**
Once the structure was in place, I started making the grid functional. I added the key methods in `Grid.cpp` such as:

- `clearAll`
- `clearCell`
- `setStart`
- `addGoal`
- `toggleObstacle`
- `clearPath`
- `markPath`

This was the point where the project stopped just being an shell and actually started behaving like a real environment. It also made the car park idea more obvious, because I could now support multiple goals to mimic multiple free spaces.

I also updated `main.cpp` so that commands like `START`, `GOAL`, `OBST`, `CLEAR`, `CLEARALL`, and `RUN` would call the correct grid functions and immediately reprint the grid. That made testing much easier.

### **Implementing the actual A* logic**
After that, I moved on to `AStar.cpp` because `RUN` did not really mean anything without the algorithm in place.

I added a `Node` structure to store:

- the current position
- the `g` score
- the `h` score

I also added the `f()` function to return the total score so that the priority queue could always sort nodes by the most promising option.

I used:

- a priority queue for the open list
- a `gScore` map to store the best known cost to each cell
- a `cameFrom` map to reconstruct the final path

At this stage I used Manhattan distance because movement was still limited to four directions, so that was the most sensible starting point, but later David Jayakumar convinced me to expand to other Heuristics.

### **Fixing issues properly instead of ignoring them**
Once I started testing more, I found a few important problems.

One bug was in `setStart()`. I realised that if I placed the start node on top of a cell that had already been marked as a goal, the cell would visually become the start but would still remain in the goals vector. That meant the code could still treat the start as a goal, which made it look like the algorithm had not really run at all. I fixed this by updating `setStart()` so it removes that goal from the vector before placing the start.

Another issue came from invalid input. If I typed something like `START AA`, the program could crash because `stoi` would throw an exception when trying to parse the row part. I fixed this by checking the format first and wrapping the conversion in a `try/catch` block so the command would fail normally instead of crashing the whole program.

These were small issues, but they were exactly the kind of things that could damage the project badly during a demonstration if left unfixed.

### **Improving the console display**
After the main logic was working, I made the display look better because it was becoming annoying to test pathfinding in a plain unreadable grid.

I added ANSI colours so that:

- obstacles appear in red
- the start appears in blue
- goals appear in green
- the path appears in yellow

This did not change the algorithm, but it made the program much easier to read and made testing much more comfortable.

### **Adding a default car park layout**
I also got tired of manually building the same parking layout every time I ran the program, so I added `loadDefaultLayout()` to automatically create a car park style layout. This made the project feel much closer to its intended use and also made it quicker to test repeatedly.

### **Final polishing**
Towards the end, I added extra usability features such as a help menu, clearer feedback messages, and a better command experience overall. By that point, the console version felt polished enough to be submitted and also ready to be used as the route finding layer for my final year project later on.

## **Heuristics and Why I Experimented With Them**

Originally I used Manhattan distance because the movement model only allowed up, down, left, and right. Since those were the allowed moves, Manhattan was the most suitable option at that stage.

However, because this project is intended to support my Smart Parking Car Park system, I wanted to experiment further rather than just stopping at the first working version. I wanted to see whether another heuristic might suit the project better depending on how movement is interpreted in the car park model.

The heuristics I tested were:

- **Manhattan**
- **Euclidean**
- **Chebyshev**

### **Manhattan**
This was the original heuristic and made sense because the early version of the project used four-direction movement only. It was simple, efficient, and matched the movement rules directly.

### **Euclidean**
I then added Euclidean distance because it gives a more natural straight-line estimate. Since this project is meant to help guide a route visually in a parking environment, I wanted to see how it behaved when diagonal movement was also allowed.

### **Chebyshev**
I also added Chebyshev distance as another comparison option when diagonal movement is valid.

One thing I noticed during testing was that changing the heuristic alone did not automatically make the route look diagonal. At first that confused me, because I expected Euclidean or Chebyshev to naturally produce different looking paths. Then I realised that the issue was not the heuristic formula itself. The movement rules were still only allowing four-direction neighbours.

Once I understood that, I updated the code so that Manhattan could only do four-direction movement while Euclidean and Chebyshev could be tested in a way that actually reflected their intended behaviour.

That experiment was important for me because I was not just adding extra features for the sake of it. I was trying to understand which behaviour makes the most sense for the eventual smart parking use case.

## **Core Algorithm Explanation**

The A* algorithm begins by placing the start node into the open list. It then repeatedly selects the node with the lowest total estimated cost and expands its neighbours.

The process is:

- add the start node to the open list
- choose the node with the lowest `f-cost`
- check whether it is one of the goals
- if not, inspect neighbouring cells
- ignore blocked or invalid cells
- update the movement cost if a better path is found
- continue until a goal is reached or no valid route remains

Once the algorithm reaches a goal, it reconstructs the final path by following the stored parent positions back toward the start.

Understanding this in code was much more useful than only understanding it in theory. Having to manage the open list, the scores, and the path reconstruction made the logic of the algorithm much clearer to me.

## **Modern C++ Features and Key Code Breakdown**

This section walks through some of the more technically interesting parts of the implementation. These are the lines and patterns I had to think carefully about when writing them, and I want to explain what they are doing and why they are written the way they are.

---

### **The Priority Queue with a Custom Comparator**

```cpp
auto cmp = [](Node a, Node b) { return a.f() > b.f(); };
priority_queue<Node, vector<Node>, decltype(cmp)> openList(cmp);
```

This is one of the most important lines in the whole project and also one of the more C++-specific ones.

A `priority_queue` in C++ is a data structure that always gives you the highest priority element first. By default it is a **max-heap**, meaning the largest value comes out first. But for A*, I need the node with the **lowest** f-cost to come out first, not the highest. So the comparator has to be flipped.

The lambda `[](Node a, Node b) { return a.f() > b.f(); }` is an anonymous comparison function. It says: treat `a` as lower priority than `b` if `a.f()` is greater. This flips the heap into a **min-heap**, so the node with the smallest total cost always rises to the top.

The type signature `priority_queue<Node, vector<Node>, decltype(cmp)>` breaks down as:
- `Node` — the type of element being stored
- `vector<Node>` — the underlying container used internally by the priority queue
- `decltype(cmp)` — the type of the comparator, inferred at compile time using `decltype`

Because lambda types in C++ are unique and unnamed, `decltype(cmp)` is the only way to pass the lambda's type as a template argument. The comparator itself is then passed into the constructor `openList(cmp)` so the queue knows how to order its elements at runtime.

Without this setup, the open list would always pop the worst node first, which would break the algorithm entirely.

---

### **Timing with `chrono::high_resolution_clock`**

```cpp
auto startTime = chrono::high_resolution_clock::now();

// ... algorithm runs ...

auto endTime = chrono::high_resolution_clock::now();
result.stats.executionMicros =
    chrono::duration_cast<chrono::microseconds>(endTime - startTime).count();
```

This uses the `<chrono>` library from modern C++ to measure exactly how long each search takes.

`chrono::high_resolution_clock::now()` captures the current time as a **time point**. The start is recorded before the algorithm begins, and the end is recorded after it finishes or exits early. Subtracting the two gives a **duration** object representing the elapsed time.

`chrono::duration_cast<chrono::microseconds>` converts that duration into microseconds. The `.count()` call extracts the raw integer value, which gets stored in the result stats.

You'll notice `endTime` is captured at every early-exit point in the function, not just at the end. That's intentional — if the function returns early because no start was set, or because the start is already a goal, the timing still needs to be accurate for that case. Every return path captures its own end time before leaving.

Microseconds was chosen as the unit because A* on a small grid runs very fast. Milliseconds would often just report 0 for short runs, which would make the heuristic performance comparison meaningless.

---

### **The Three Heuristics — Formulas and How They Work**

The heuristic estimates the remaining distance from a given cell to the goal. All three heuristics are scaled by a factor of 10 to match the movement costs used elsewhere (straight moves cost 10, diagonal moves cost 14).

```cpp
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
```

#### Manhattan Distance

```
h = (|Δrow| + |Δcol|) × 10
```

Manhattan adds the absolute row difference and the absolute column difference. The name comes from the street grid of Manhattan — you can only travel along axes, never diagonally. This is the right heuristic when movement is locked to **four directions only**. It never overestimates the true cost in that model, which is what makes A* optimal.

**Example:** current = (2, 1), goal = (5, 4)
```
rowDiff = 3, colDiff = 3
h = (3 + 3) × 10 = 60
```

#### Euclidean Distance

```
h = √(Δrow² + Δcol²) × 10
```

Euclidean distance is the straight-line distance between two points. It suits movement models where **diagonal movement is allowed**. The result is multiplied by 10 and rounded to the nearest integer. The `static_cast<double>` is needed because `sqrt` requires a floating-point input, and the final cast back to `int` keeps everything consistent with the rest of the scoring.

**Example:** current = (2, 1), goal = (5, 4)
```
rowDiff = 3, colDiff = 3
h = √(9 + 9) × 10 = √18 × 10 ≈ 42
```

Because the straight-line distance is always shorter than or equal to Manhattan when diagonals are available, Euclidean gives a tighter and more direct estimate.

#### Chebyshev Distance

```
h = max(|Δrow|, |Δcol|) × 10
```

Chebyshev takes the larger of the two axis differences. The logic is: if diagonal moves cost the same as straight moves, you can always cover the smaller axis during your diagonal steps. So the minimum number of moves is just whichever axis is further.

**Example:** current = (2, 1), goal = (5, 4)
```
rowDiff = 3, colDiff = 3
h = max(3, 3) × 10 = 30
```

In this project, Chebyshev diagonal moves are costed at 10 (same as straight) rather than 14, which reflects the equal-cost assumption in that movement model.

#### Why the Heuristic Alone Doesn't Change the Path Shape

This is something I had to figure out the hard way. The heuristic does not change the route by itself — what changes the route is the **movement model**. The code uses a helper to decide which model applies:

```cpp
static bool usesDiagonalMovement(HeuristicType heuristicType) {
    return heuristicType == HeuristicType::Euclidean || heuristicType == HeuristicType::Chebyshev;
}
```

Manhattan locks the search to four directions. Euclidean and Chebyshev both unlock eight-direction movement. Switching the heuristic also switches the movement model, which is what actually produces the different looking paths.

---

### **Path Reconstruction — How the Route Is Stored and Retrieved**

Once the algorithm reaches a goal, it traces the route back to the start using the `cameFrom` map.

#### How `cameFrom` Works

```cpp
unordered_map<int, Position> cameFrom;
```

Every time a node is reached via a better path, its parent is recorded:

```cpp
cameFrom[nextKey] = curr.pos;
```

The key is an integer produced by `positionKey`:

```cpp
static int positionKey(const Position& pos) {
    return pos.row * Grid::COLS + pos.col;
}
```

This flattens a 2D coordinate into a single unique integer. On a 10-column grid, position (3, 4) becomes `3 × 10 + 4 = 34`. This is a standard technique for using 2D positions as hash map keys without needing a custom hash function.

`unordered_map` is used rather than `map` because it gives O(1) average lookup time instead of O(log n). When the algorithm is doing thousands of lookups during a search, that difference adds up.

#### How the Path Is Reconstructed

```cpp
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
```

This starts at the goal and follows parent pointers backwards through the map. Each step adds the current position to the vector and jumps to whoever recorded it as their child. The loop stops when there is no parent entry for the current position — that is the start node, which was never added to `cameFrom`.

The vector ends up in reverse order (goal → start), so `reverse` is called before returning it. The corrected path is then used by the grid to mark and display the final route, and its length is written into the performance stats.
**

## **Code Example 1: File Structure and Main Setup**

<img src="assets/images/main-setup.png" alt="Main setup showing project structure and command handling">

**Figure 1.** Main setup showing the project structure and command handling.

## **Code Example 2: Grid Logic**

<img src="assets/images/grid-logic1.png" alt="Grid logic screenshot showing start and goal handling">
<img src="assets/images/grid-logic2.png" alt="Grid logic screenshot showing coordinate parsing and validation">

**Figure 2.** Grid logic used to manage coordinates, obstacles, starts, and goals.

## **Code Example 3: A* Core Logic**

<img src="assets/images/astar-core1.png" alt="A* core logic screenshot showing node structure and scoring">
<img src="assets/images/astar-core2.png" alt="A* core logic screenshot showing heuristic and setup">
<img src="assets/images/astar-core3.png" alt="A* core logic screenshot showing main search loop">
<img src="assets/images/astar-core4.png" alt="A* core logic screenshot showing neighbour expansion and path reconstruction">

**Figure 3.** Core A* implementation showing the search process and final path reconstruction.

## **Code Example 4: Heuristic and Review Features**

<img src="assets/images/heuristic-review.png" alt="Heuristic selection and review feature screenshot">

**Figure 4.** Final extension showing heuristic selection and performance comparison support.

## **Program Output**

<img src="assets/images/program-run.png" alt="Program output showing pathfinding result">

**Figure 5.** Example program output showing the start node, goals, obstacles, and final route.

## **Testing and Validation**

Testing was important because this project is meant to support a wider system, so it had to behave properly in both normal and edge-case situations.

### **Test Case 1: Normal route available**
A standard grid with a reachable goal was used.

**Expected result:**  
A valid path should be found and displayed.

**Actual result:**  
The algorithm successfully returned a path.

### **Test Case 2: Multiple goals available**
A layout with several parking goals was tested.

**Expected result:**  
The algorithm should find a route to a reachable parking space.

**Actual result:**  
The program successfully returned a valid path to one of the goals.

### **Test Case 3: Invalid input**
Commands with invalid coordinates were tested.

**Expected result:**  
The program should fail safely without crashing.

**Actual result:**  
After updating the input parsing, the command now returns an error message instead of crashing.

### **Test Case 4: No path possible**
A blocked layout was tested where no route existed.

**Expected result:**  
The program should report that no path was found.

**Actual result:**  
The case was handled correctly.

### **Test Case 5: Heuristic comparison**
The same layout was tested with Manhattan, Euclidean, and Chebyshev.

**Expected result:**  
The search behaviour or route shape may differ depending on the movement rules and heuristic used.

**Actual result:**  
The results varied, which helped me compare which behaviour seemed most suitable for the intended project use.

## **Performance Review**

Since this project is meant to support the Smart Parking Car Park system, I wanted to go beyond simply proving that the algorithm worked. I also wanted to compare how it behaved.

I added a performance overview feature that outputs:

- the heuristic used
- whether a path was found
- the chosen goal
- the path length
- the number of nodes expanded
- the number of nodes generated
- the execution time in microseconds

This made the project stronger because it allowed me to compare the heuristics in a more structured way and think more seriously about which one best suits the intended application.

<img src="assets/images/performance-review.png" alt="Performance review comparison output">

**Figure 6.** Performance comparison between different heuristics on the same car park layout.

## **Problems Encountered and How I Solved Them**

One problem I encountered was the issue where the start node could overwrite a goal visually but still leave that goal stored in memory. I fixed this by updating `setStart()` to remove the goal from the vector before setting the start.

Another issue was invalid input crashing the program. This was caused by coordinate parsing and `stoi`. I fixed this by improving the validation and using `try/catch` to prevent the application from crashing.

A more interesting issue was understanding why different heuristics were not changing the route the way I expected. This forced me to think more carefully about the difference between the heuristic formula and the movement model. Once I realised that, I could make changes that actually improved the project instead of just guessing.

## **Reflection**

This project helped me understand A* much more deeply than just reading about it. Before implementing it, I understood the general idea of pathfinding, but writing the code, tracking the costs, reconstructing the path, and debugging the behaviour made the algorithm much clearer.

I also think the project became stronger because I developed it with my final year project in mind. That gave the work a clear purpose. Instead of building a generic algorithm demo, I was building a route-finding layer that can later sit on top of my Smart Parking Car Park system.

Another thing that helped was keeping development logs while I worked. Looking back at them, I can clearly see the project progressing from basic file setup, to a working grid, to a proper A* implementation, and then finally to something polished enough to support integration into the wider pipeline.

If I were to improve the project further, I would like to:

- connect the grid directly to detected parking space data
- generate routes from real parking layouts automatically
- add weighted movement costs for a more realistic navigation model
- improve the visualisation beyond the console
- integrate the final route overlay into the full smart parking pipeline

Overall, I believe the project was successful because it not only produced a working A* implementation, but also created a meaningful pathfinding layer for my wider Smart Parking Car Park final year project.

## **References**

1. cppreference.com (2025) std::priority_queue.

2. cppreference.com (2025) std::unordered_map.

3. cppreference.com (2024) std::chrono::high_resolution_clock.

4. cppreference.com (2024) std::chrono::duration_cast.

5. https://claude.ai

6. https://chatgpt.com/

7. https://www.redblobgames.com/pathfinding/a-star/introduction.html

8. Atlantic Technological University (2026) C++ Programming Project Brief and Rubric.
