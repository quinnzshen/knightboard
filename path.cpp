#include "path.h"
using namespace std;

Path::Path() {
  totalWeight = 0;
}

Path::Path(Position position, int positionId) {
  moves.push_back(position);
  visited.insert(positionId);
  totalWeight = position.weight;
}

bool Path::newMove(Position position, int positionId) {
  if (visited.find(positionId) != visited.end()) {
    return false;
  }

  moves.push_back(position);
  visited.insert(positionId);
  totalWeight += position.weight;

  return true;
}

bool operator==(Path path1, Path path2) {
  return path1.moves.back() == path2.moves.back() && path1.moves.front() == path2.moves.front();
}

bool operator<(Path path1, Path path2) {
  return path1.totalWeight > path2.totalWeight;
}
