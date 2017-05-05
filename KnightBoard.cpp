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

vector<Position> getValidMoves(Position position, int boardLength) {
  vector<Position> potentialMoves;

  potentialMoves.push_back(Position(position.row + 2, position.col + 1));
  potentialMoves.push_back(Position(position.row + 2, position.col - 1));
  potentialMoves.push_back(Position(position.row - 2, position.col + 1));
  potentialMoves.push_back(Position(position.row - 2, position.col - 1));
  potentialMoves.push_back(Position(position.row + 1, position.col + 2));
  potentialMoves.push_back(Position(position.row + 1, position.col - 2));
  potentialMoves.push_back(Position(position.row - 1, position.col + 2));
  potentialMoves.push_back(Position(position.row - 1, position.col - 2));

  vector<Position> validMoves;
  for (Position move : potentialMoves) {
    if (isValidKnightMove(position, move, boardLength)) {
      validMoves.push_back(move);
    }
  }

  return validMoves;
}

vector<vector<int>> createAdjacencyList(int boardLength) {
  vector<vector<int>> adjacencyList;

  for (int row = 0; row < boardLength; row++) {
    for (int col = 0; col < boardLength; col++) {
      vector<Position> validMoves = getValidMoves(Position(row, col), boardLength);
      cout << "rc:" << row << " " << col << " size: " << validMoves.size() << endl;
      vector<int> validMovesId;

      for (Position move : validMoves) {
        cout << "move: " << move.row << " " << move.col << " id: " << idFromPosition(move, boardLength) << endl;
        validMovesId.push_back(idFromPosition(move, boardLength));
      }

      adjacencyList.push_back(validMovesId);
    }
  }

  return adjacencyList;
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

  // Testing getValidMoves
  vector<Position> validMoves = getValidMoves(Position(0, 0), 3);
  for (Position move : validMoves) {
    if (!((move.row == 2 && move.col == 1) || (move.row == 1 && move.col == 2))) {
      cout << "testGetValidMoves Failure" << endl;
    }
  }

  vector<vector<int>> adjacencyList = createAdjacencyList(3);
  for (int nodeId = 0; nodeId < 9; nodeId++) {
    cout << nodeId << ": ";
    vector<int> node = adjacencyList.at(nodeId);
    for (int id : node) {
      cout << id << " ";
    }
    cout << endl;
  }
}