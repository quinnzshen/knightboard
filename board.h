#ifndef BOARD_H
#define BOARD_H

#include "position.h"
#include "move.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <iomanip>
#include <fstream>
#include <unordered_set>
#include <cassert>
using namespace std;

struct Path {
  vector<Position> moves;
  unordered_set<int> visited;
  int totalWeight;

  Path() {
    totalWeight = 0;
  }

  Path(Position position, int positionId) {
    moves.push_back(position);
    visited.insert(positionId);
    totalWeight = position.weight;
  }

  bool newMove(Position position, int positionId) {
    if (visited.find(positionId) != visited.end()) {
      return false;
    }

    moves.push_back(position);
    visited.insert(positionId);
    totalWeight += position.weight;

    return true;
  }

  friend bool operator<(Path path1, Path path2) {
    return path1.totalWeight < path2.totalWeight;
  }
};

struct Weight {
  Path path;
  int estimatedCost;

  Weight(Path path, int cost) {
    this->path = path;
    estimatedCost = path.totalWeight + cost;
  }

  friend bool operator<(Weight weight1, Weight weight2) {
    return weight1.estimatedCost < weight2.estimatedCost;
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
  Path longestPath(int startId, int goalId);

  void printBoard(Position startingPosition, Position endingPosition, Path path);
  void printBoard(Position startingPosition, Position endingPosition, Position currentPosition);
  void printBoardId();
};

#endif
