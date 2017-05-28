#include "move.h"
using namespace std;

Move::Move(Position current, Position parent, int weight) {
  position = current;
  parentPosition = parent;
  totalWeight = weight;
}

Move Move::newMove(Position newPosition) {
  return Move(newPosition, position, totalWeight + newPosition.weight);
}

bool operator==(Move move1, Move move2) {
  return move1.position == move2.position;
}

bool operator<(Move move1, Move move2) {
  return move1.totalWeight > move2.totalWeight;
}
