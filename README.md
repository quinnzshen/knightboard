# knightboard
To compile & run main.cpp (challenges lvl 3-5):
```
clang++ -std=c++1z -stdlib=libc++ position.cpp move.cpp priority_queue.cpp path.cpp heuristic.cpp board.cpp main.cpp -o main
./main
```

To compile test.cpp (unit-tests):
```
clang++ -std=c++1z -stdlib=libc++ position.cpp move.cpp priority_queue.cpp path.cpp heuristic.cpp board.cpp test.cpp -o test
./test
```

Modify the main.cpp as you wish (Note: settings the `DEFAULT_SLEEP_MS` parameter below 0ms will result in no sleep or printing of intermediate board steps / explorations).

The first two parameters for board.dijkstra and board.longestPath is `(startId, goalId)`.
```c++
#include "board.h"
using namespace std;

const int DEFAULT_EXPLORE_DEPTH = 1200;
const int DEFAULT_SLEEP_MS = 100;

int main() {
  // Level 3
  Board board = Board("boards/lvl3Board.txt");
  board.isValidSequence(board.dijkstra(17, 37));

  cout << "Completed level 3. Press Enter to Continue:" << endl;
  cin.ignore();

  // Level 4
  board = Board("boards/board.txt");
  board.isValidSequence(board.dijkstra(13, 856));

  cout << "Completed level 4. Press Enter to Continue:" << endl;
  cin.ignore();

  // Level 5
  board.isValidSequence(board.longestPath(13, 856, DEFAULT_EXPLORE_DEPTH, DEFAULT_SLEEP_MS));

  cout << "Completed level 5." << endl;
}
```

**Level 3**:
To implement level 3, I began with a simple Breadth First Search (BFS) treating this as an unweighted undirected graph problem. This would give me a valid sequence from a given start point to a given end point in the fewest number of moves.

**Level 4**:
Level 4 turned our level 3 problem into an undirected weighted graph problem. The way I thought about it was that each position (on the board) would map to a list of positions that it could jump to (given the constraints of rocks + barriers + boundaries) with an associated weight / cost as the edge length. The weight would represent how many 'turns' it took to jump on the weight. E.g. Landing in water cost 2 turns, therefore the edge weight connecting to a water position would be 2. I chose to represent this as an adjacency list since I figured it would be relatively sparse. To solve this problem, I implemented Dijkstra's algorithm utilizing a priority queue.

**Level 5**:
For level 5, we wanted to find the longest path in an undirected weighted graph. I explored and iterated on many different ways of solving this before coming up with a solution that I was pretty satisfied with. If our weighted graph were directed, we could run something like Bellman-Ford Algorithm by negating all of the weights to find the longest path; however, since we were dealing with an undirected graph we aren't able to do so. 

My first intuition was to try and brute-force a solution using Depth First Search (DFS), continuiously searching and exploring paths until we either explored the entire graph (and returned the longest path) or once we got a 'good enough solution'. While watching my algorithm explore - I noticed that my naive DFS was unnecessarily exploring a lot of dead ends (i.e. it would trap itself in a corner and continue exploring all possible unvisited nodes. 

My second iteration utilized this observation and would only consider a new move if we could verify that there still existed a path to the goal without utilizing any of the already visited positions. This pruning drastically sped up our search. One thing that I noticed in this iteration was that it would sometimes be exploring around a goal node, even when other parts of the board hadn't yet been explored. I tried a few additional ideas (e.g. random shuffling of valid moves, trying to encourage an outward spiral) before coming up with my final solution.

My final solution tried to gauge the value of a move before exploring it. More specifically:
```
priority_within_queue = currentPath.totalWeight + estimatedCostToGoal;
```

By creating this heuristic, we were now prioritizing the exploration of paths that would also take us further away from the goal id. This encouraged the Best-First-Search (A\*) to explore as far as possible (exploring all corners of the board) before reaching the goal. The best way to estimate my cost to goal was to utilize my Djikstra's algorithm and determine how many moves (vs. weight) it would take me reach the goal in the current state. 

# Additional Notes:
I allow multiple teleporters in my implementation; the visited position is the position you teleport to (not the space used to teleport). For example, if there are three Teleporters A, B, C; you can use A to teleport to B, then A to teleport to C, and either B or C to teleport back to A.

When printing a path, I opt'ed to show teleportation tiles even after they've been visited (for the reasons mentioned above). This way you can still visually see where a teleport tile is.
