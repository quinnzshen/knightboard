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

struct Moves {
  vector<Position> moves;
  unordered_set<int> visited;
  int totalWeight;

  Moves() {
    totalWeight = 0;
  }

  Moves(Position position, int positionId) {
    moves.push_back(position);
    visited.insert(positionId);
    totalWeight = position.weight;
  }

  bool newMove(Position position, int positionId) {
    // for (auto elem : visited) {
    //   cout << elem << " ";
    // }
    // cout << "!" << endl;

    if (visited.find(positionId) != visited.end()) {
      return false;
    }

    moves.push_back(position);
    visited.insert(positionId);
    totalWeight += position.weight;

    return true;
  }

  friend bool operator<(Moves moves1, Moves moves2) {
    return moves1.totalWeight > moves2.totalWeight;
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
  bool isValidSequence(vector<Position> sequence);

  vector<Position> getValidMoves(int row, int col);
  vector<Position> getValidMoves(Position position);

  vector<vector<Position>> getAdjacencyList();
  vector<Position> dijkstra(int startId, int goalId, unordered_set<int> visited = unordered_set<int>());
  Moves longestPath(int startId, int goalId);

  void printBoard(Position startingPosition, Position endingPosition, Moves path);
  void printBoard(Position startingPosition, Position endingPosition, Position currentPosition);
  void printBoardId();
};

#endif
