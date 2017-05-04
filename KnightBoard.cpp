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

bool isValidPosition(Position position, int boardLength) {
  return (position.row >= 0) && (position.row < boardLength) && (position.col >= 0) && (position.col < boardLength);
}

bool isValidKnightMove(Position startPosition, Position endPosition, int boardLength) {
  if (!isValidPosition(startPosition, boardLength) || !isValidPosition(endPosition, boardLength)) {
    return false;
  }

  int deltaRow = endPosition.row - startPosition.row;
  int deltaCol = endPosition.col - startPosition.col;

  return (abs(deltaRow) == 1 && abs(deltaCol) == 2) || (abs(deltaRow) == 2 && abs(deltaCol) == 1);
}

int main() {
  // Testing positionFromId
  Position position = positionFromId(4, 3);
  if (position.row == 1 && position.col == 1) {
    cout << "testPositionFromId() Success" << endl;
  }

  // Testing idFromPosition
  int id = idFromPosition(Position(1, 1), 3);
  if (id == 4) {
    cout << "testIdFromPosition() Success" << endl;
  }

  // Testing isValidPosition
  bool validPosition = isValidPosition(Position(1, 1), 3);
  if (validPosition) {
    cout << "testValidPosition() Success" << endl;
  }

  validPosition = isValidPosition(Position(3, 1), 3);
  if (!validPosition) {
    cout << "testInvalidPosition() Success" << endl;
  }

  // Testing isValidKnightMove
  bool validKnightMove = isValidKnightMove(Position(0, 0), Position(1, 2), 3);
  if (validKnightMove) {
    cout << "testValidKnightMove() Success" << endl;
  }

  validKnightMove = isValidKnightMove(Position(0, 0), Position(1, 1), 3);
  if (!validKnightMove) {
    cout << "testInvalidKnightMove() Success" << endl;
  }

  validKnightMove = isValidKnightMove(Position(0, 0), Position(-1, -2), 3);
  if (!validKnightMove) {
    cout << "testInvalidKnightMoveOutOfBoard() Success" << endl;
  }
}