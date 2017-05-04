//============================================================================
// Name        : KnightBoard.cpp
// Author      : Quinn Z Shen
// Version     :
// Copyright   :
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <array>
#include <tuple>
#include <vector>
#include <stack> 
#include <iomanip> 
using namespace std;

struct Position {
  int row, col;

  Position(int row, int col) {
    this->row = row;
    this->col = col;
  }
};

Position positionFromId(int id, int boardLength) {
  int row = id / boardLength;
  int col = id % boardLength;

  return Position(row, col);
}

int idFromPosition(Position position, int boardLength) {
  return position.row * boardLength + position.col;
}

int main() {
  // Testing positionFromId
  Position position = positionFromId(4, 3);
  if (position.row == 1 && position.col == 1) {
    cout << "positionFromId() Test Success." << endl;
  }

  // Testing idFromPosition
  int id = idFromPosition(Position(1, 1), 3);
  if (id == 4) {
    cout << "idFromPosition() Test Success." << endl;
  }
}