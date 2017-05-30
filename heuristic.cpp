#include "heuristic.h"
using namespace std;

Heuristic::Heuristic(Path path, int estimatedCost) {
  this->path = path;
  this->estimatedCost = path.totalWeight + estimatedCost;
}

bool operator==(Heuristic heuristic1, Heuristic heuristic2) {
  return heuristic1.path == heuristic2.path;
}

bool operator<(Heuristic heuristic1, Heuristic heuristic2) {
  return heuristic1.estimatedCost < heuristic2.estimatedCost;
}
