#ifndef POSITION_H
#define POSITION_H

#include <limits>
using namespace std;

enum Type { WATER, ROCK, BARRIER, TELEPORT, LAVA, NORMAL };
const int MAX_WEIGHT = numeric_limits<int>::max();

/* Defines a Position */
struct Position {
  int row, col, weight;
  Type type;

  Position(int row, int col, int weight);
  Position(int row, int col, char ch = '.');

  bool isBarrier();

  int idFromPosition();

  friend bool operator==(Position pos1, Position pos2);
  friend bool operator!=(Position pos1, Position pos2);
  friend bool operator<(Position pos1, Position pos2);
};

#endif
