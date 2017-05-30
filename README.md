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

Level 3:
To implement level 3, I began with a simple Breadth First Search (BFS) treating this as an unweighted undirected graph problem. This would give me a valid sequence from a given start point to a given end point in the fewest number of moves.

Level 4:
Level 4 turned our level 3 problem into an undirected weighted graph problem. The way I thought about it was that each position (on the board) would map to a list of positions that it could jump to (given the constraints of rocks + barriers + boundaries) with an associated weight / cost as the edge length. The weight would represent how many 'turns' it took to jump on the weight. E.g. Landing in water cost 2 turns, therefore the edge weight connecting to a water position would be 2. I chose to represent this as an adjacency list since I figured it would be relatively sparse. To solve this problem, I implemented Dijkstra's algorithm utilizing a priority queue.

Level 5:
For level 5, we wanted to find the longest path in an undirected weighted graph. I explored and iterated on many different ways of solving this before coming up with a solution that I was pretty satisfied with. If our weighted graph were directed, we could run something like Bellman-Ford Algorithm by negating all of the weights to find the longest path; however, since we were dealing with an undirected graph I didn't think we would be able to do so. My first intuition was to try and brute-force a solution using Depth First Search (DFS), continuiously searching and exploring paths until we either explored the entire graph (and returned the longest path) or once we got a 'good enough solution'. While watching my algorithm explore - I noticed that my naive DFS was exploring a lot of dead ends (i.e. it would trap itself in a corner and continue exploring all possible unvisited nodes. My second iteration utilized this observation; I would only consider a new move, if we were positive that there still existed a way (without utilizing any of the already visited nodes) to get to the birthday. This iteration worked much better, however, I noticed that while we were only exploring meaningful 'valid' paths - performance varied a lot depending on where the start goal and end goals were; sometimes we would get explore  a corner with the goal surrounding it before exploring other sections of the map. I tried a few additional ideas (e.g. random shuffling of valid moves, trying to encourage an outward spiral) before coming up with my final solution.

My final solution tried to gauge the value of a move before exploring it. More specifically:
```
priority_within_queue = currentPath.totalWeight + estimatedCostToGoal;
```

By creating this heuristic, we were now prioritizing the exploration of paths that would also take us further away from the goal id. This encouraged the Best-First-Search (A\*) to explore as far as possible (exploring all nooks within the board) before finally settling with the Goal.
