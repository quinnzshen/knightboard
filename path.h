#ifndef PATH_H
#define PATH_H

#include "position.h"
#include <vector>
#include <unordered_set>
using namespace std;

/* A Path allows us to keep track of a move sequence, visited ids, and the total weight */
struct Path {
  vector<Position> moves;
  unordered_set<int> visited;
  int totalWeight;

  Path();
  Path(Position position, int positionId);

  bool newMove(Position position, int positionId);

  friend bool operator==(Path path1, Path path2);
  friend bool operator<(Path path1, Path path2);
};

#endif
