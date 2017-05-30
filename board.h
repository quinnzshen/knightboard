#ifndef BOARD_H
#define BOARD_H

#include "position.h"
#include "move.h"
#include "priority_queue.h"
#include "path.h"
#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>
#include <unordered_set>
#include <cassert>
using namespace std;

struct Heuristic {
  Path path;
  int estimatedCost;

  Heuristic(Path path, int estimatedCost) {
    this->path = path;
    this->estimatedCost = path.totalWeight + estimatedCost;
  }

  friend bool operator==(Heuristic heuristic1, Heuristic heuristic2) {
    return heuristic1.path == heuristic2.path;
  }

  friend bool operator<(Heuristic heuristic1, Heuristic heuristic2) {
    return heuristic1.estimatedCost < heuristic2.estimatedCost;
  }
};

class Board {
  vector<vector<Position>> board;
  unordered_set<int> teleportPositionIds;
public:
  Board(string fileName);
  int size();

  Position positionFromId(int id);
  int idFromPosition(Position position);

  bool isValidPosition(int row, int col);
  bool isBarrierBlocking(Position start, Position end);
  bool isValidKnightMove(Position start, Position end);
  bool isValidTeleport(Position start, Position end);
  bool isValidSequence(Path path);
  bool isValidSequence(vector<Position> sequence);

  vector<Position> getValidPath(int row, int col);
  vector<Position> getValidPath(Position position);

  vector<vector<Position>> getAdjacencyList();
  vector<Position> dijkstra(int startId, int goalId, unordered_set<int> visited = unordered_set<int>());
  Path longestPath(int startId, int goalId, int exploreDepth);

  void printBoard(Position startingPosition, Position endingPosition, Path path);
  void printBoard(Position startingPosition, Position endingPosition, Position currentPosition);
  void printBoardId();
};

#endif
