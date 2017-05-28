#ifndef MOVE_H
#define MOVE_H

#include "position.h"

const Position POSITION_BEGIN = Position(-1, -1, 0);

struct Move {
  Position position = POSITION_BEGIN;
  Position parentPosition = POSITION_BEGIN;
  int totalWeight;

  Move(Position current, Position parent, int weight);
  Move newMove(Position newPosition);

  friend bool operator==(Move move1, Move move2);
  friend bool operator<(Move move1, Move move2);
};

#endif
