#include "position.h"
using namespace std;

Position::Position(int row, int col, int weight) {
  this->row = row;
  this->col = col;
  this->weight = weight;
}

Position::Position(int row, int col, char ch) {
  this->row = row;
  this->col = col;

  switch(ch) {
    case 'W':
      type = WATER;
      weight = 2;
      break;
    case 'R':
      type = ROCK;
      weight = MAX_WEIGHT;
      break;
    case 'B':
      type = BARRIER;
      weight = MAX_WEIGHT;
      break;
    case 'T':
      type = TELEPORT;
      weight = 1;
      break;
    case 'L':
      type = LAVA;
      weight = 4;
      break;
    default:
      type = NORMAL;
      weight = 1;
      break;
  }
}

bool Position::isBarrier() {
  return type == BARRIER;
}

int Position::idFromPosition() {
  return this->row * 8 + this->col;
}

bool operator==(Position pos1, Position pos2) {
  return (pos1.row == pos2.row) && (pos1.col == pos2.col);
}

bool operator!=(Position pos1, Position pos2) {
  return (pos1.row != pos2.row) || (pos1.col != pos2.col);
}

bool operator<(Position pos1, Position pos2) {
  return pos1.weight > pos2.weight;
}
