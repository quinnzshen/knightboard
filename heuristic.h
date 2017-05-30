#ifndef HEURISTIC_H
#define HEURISTIC_H

#include "path.h"
using namespace std;

struct Heuristic {
  Path path;
  int estimatedCost;

  Heuristic(Path path, int estimatedCost);

  friend bool operator==(Heuristic heuristic1, Heuristic heuristic2);
  friend bool operator<(Heuristic heuristic1, Heuristic heuristic2);
};

#endif
