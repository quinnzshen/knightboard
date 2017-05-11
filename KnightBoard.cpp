//============================================================================
// Name        : KnightBoard.cpp
// Author      : Quinn Z Shen
// Version     :
// Copyright   :
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <vector>
#include <queue>
#include <iomanip> 
#include <fstream>
using namespace std;

struct Position {
  int row, col;

  Position(int row, int col) {
    this->row = row;
    this->col = col;
  }

  friend bool operator==(Position pos1, Position pos2) {
    return (pos1.row == pos2.row) && (pos1.col == pos2.col);
  }
};

enum Type { WATER, ROCK, BARRIER, TELEPORT, LAVA, NORMAL };

struct GridTile {
  Position position = Position(0, 0);;
  Type type;

  GridTile(int row, int col, char type) {
    this->position.row = row;
    this->position.col = col;

    switch(type) {
      case 'W':
        this->type = WATER;
        break;
      case 'R':
        this->type = ROCK;
        break;
      case 'B':
        this->type = BARRIER;
        break;
      case 'T':
        this->type = TELEPORT;
        break;
      case 'L':
        this->type = LAVA;
        break;
      default:
        this->type = NORMAL;
        break;
    }
  }

  Type getType() {
    return this->type;
  }

  Position getPosition() {
    return this->position;
  }
};

vector<vector<GridTile>> readMapFromFile(string fileName) {
  vector<vector<GridTile>> board;
  fstream infile(fileName);
  char ch;
  int row = 0;
  int col = 0;

  while (infile >> noskipws >> ch) {
    if (ch == '\n') {
      row++;
      col = 0;
      continue;
    }

    if (col == 0) {
      vector<GridTile> vectorRow;
      board.push_back(vectorRow);
    }

    board[row].push_back(GridTile(row, col, ch));
    col++;
  }

  return board;
}

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

bool isValidPosition(Position position, vector<vector<GridTile>> board) {
  int row = position.row;
  int col = position.col; 
  Type type = board[row][col].getType();

  return (row >= 0) && (row < board.size()) && (col >= 0) && (col < board[row].size()) && (type != ROCK) && (type != BARRIER);
}

bool isValidKnightMove(Position startPosition, Position endPosition, int boardLength) {
  if (!isValidPosition(startPosition, boardLength) || !isValidPosition(endPosition, boardLength)) {
    return false;
  }

  int deltaRow = endPosition.row - startPosition.row;
  int deltaCol = endPosition.col - startPosition.col;

  return (abs(deltaRow) == 1 && abs(deltaCol) == 2) || (abs(deltaRow) == 2 && abs(deltaCol) == 1);
}

bool isValidKnightMove(Position startPosition, Position endPosition, vector<vector<GridTile>> board) {
  // Ensure that starting and ending positions are both valid
  if (!isValidPosition(startPosition, board) || !isValidPosition(endPosition, board)) {
    return false;
  }

  int deltaRow = endPosition.row - startPosition.row;
  int deltaCol = endPosition.col - startPosition.col;

  // Ensure that the move is a valid knight move
  if (!((abs(deltaRow) == 1 && abs(deltaCol) == 2) || (abs(deltaRow) == 2 && abs(deltaCol) == 1))) {
    return false;
  }

  if (deltaCol == 2 && (board[startPosition.row][startPosition.col + 1].getType() == BARRIER || board[startPosition.row][startPosition.col + 2].getType() == BARRIER)) {
    return false;
  } else if (deltaCol == -2 && (board[startPosition.row][startPosition.col - 1].getType() == BARRIER || board[startPosition.row][startPosition.col - 2].getType() == BARRIER)) {
    return false;
  } else if (deltaRow == 2 && (board[startPosition.row + 1][startPosition.col].getType() == BARRIER || board[startPosition.row + 2][startPosition.col].getType() == BARRIER)) {
    return false;
  } else if (deltaRow == -2 && (board[startPosition.row - 1][startPosition.col].getType() == BARRIER || board[startPosition.row - 2][startPosition.col].getType() == BARRIER)) {
    return false;
  }

  return true;
}

vector<Position> getValidMoves(Position position, vector<vector<GridTile>> board) {
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
  for (Position move: potentialMoves) {
    if (isValidKnightMove(position, move, board)) {
      validMoves.push_back(move);
    }
  }

  return validMoves;
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
      vector<int> validMovesId;

      for (Position move : validMoves) {
        validMovesId.push_back(idFromPosition(move, boardLength));
      }

      adjacencyList.push_back(validMovesId);
    }
  }

  return adjacencyList;
}

vector<Position> breadthFirstSearch(int startId, int goalId, int boardLength) {
  vector<vector<int>> adjacencyList = createAdjacencyList(boardLength);
  vector<vector<Position>> sequenceToId (boardLength * boardLength, vector<Position>());

  queue<int> queue;
  queue.push(startId);

  while (!queue.empty()) {
    int nodeId = queue.front();
    queue.pop();
    for (int validMoveId : adjacencyList.at(nodeId)) {
      if (sequenceToId.at(validMoveId).empty()) {
        queue.push(validMoveId);

        // Build the sequence that has gotten us to the current nodeId
        for (Position position : sequenceToId[nodeId]) {
          sequenceToId[validMoveId].push_back(position);
        }

        sequenceToId[validMoveId].push_back(positionFromId(nodeId, boardLength));
      }

      if (validMoveId == goalId) {
        sequenceToId[validMoveId].push_back(positionFromId(validMoveId, boardLength));
        return sequenceToId[validMoveId];
      }
    }
  }

  return vector<Position>();
}

void printBoardId(int boardLength) {
  int id = 0;
  for (int row = 0; row < boardLength; row++) {
    for (int col = 0; col < boardLength; col++) {
      cout << setw(4) << id++ << " ";
    }
    cout << endl;
  }
}

void printBoard(Position startingPosition, Position endingPosition, Position currentPosition, vector<vector<GridTile>> board) {
  cout << "Board:" << endl;
  for (vector<GridTile> row : board) {
    for (GridTile tile : row) {
      if ((startingPosition == currentPosition || endingPosition == currentPosition) && tile.position == currentPosition) {
        cout << "* ";
      } else if (tile.position == startingPosition) {
        cout << "S ";
      } else if (tile.position == endingPosition) {
        cout << "E ";
      } else if (tile.position == currentPosition) {
        cout << "K ";
      } else {
        switch(tile.type) {
          case WATER:
            cout << "W ";
            break;
          case ROCK:
            cout << "R ";
            break;
          case BARRIER:
            cout << "B ";
            break;
          case TELEPORT:
            cout << "T ";
            break;
          case LAVA:
            cout << "L ";
            break;
          default:
            cout << ". ";
            break;
        }
      }
      
    }
    cout << endl;
  }
}

void printBoard(Position startingPosition, Position endingPosition, Position currentPosition, int boardLength) {
  int id = 0;
  int startingPositionId = idFromPosition(startingPosition, boardLength);
  int endingPositionId = idFromPosition(endingPosition, boardLength);
  int currentPositionId = idFromPosition(currentPosition, boardLength);

  cout << "Board State:" << endl;
  for (int row = 0; row < boardLength; row++) {
    for (int col = 0; col < boardLength; col++) {
      if ((id == startingPositionId || id == endingPositionId) && id == currentPositionId) {
        cout << "* ";
      } else if (id == startingPositionId) {
        cout << "S ";
      } else if (id == endingPositionId) {
        cout << "E ";
      } else if (id == currentPositionId) {
        cout << "K ";
      } else {
        cout << ". ";
      }

      id++;
    }
    cout << endl;
  }
}

bool isValidSequence(vector<Position> sequence, int boardLength) {
  if (sequence.empty()) {
    return true;
  }

  for (int move = 0; move < (sequence.size() - 1); move++) {
    if (!isValidKnightMove(sequence[move], sequence[move + 1], boardLength)) {
      return false;
    }
  }

  for (int move = 0; move < sequence.size(); move++) {
    printBoard(sequence[0], sequence[sequence.size() - 1], sequence[move], boardLength);
  }

  return true;
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

  // Testing createAdjacencyList
  vector<vector<int>> adjacencyList = createAdjacencyList(8);
  for (int nodeId = 0; nodeId < 8*8; nodeId++) {
    cout << nodeId << ": ";
    vector<int> node = adjacencyList.at(nodeId);
    for (int id : node) {
      cout << id << " ";
    }
    cout << endl;
  }

  printBoardId(8);

  // Testing isValidSequence
  vector<Position> sequence;
  sequence.push_back(Position(2, 1));
  sequence.push_back(Position(4, 2));
  sequence.push_back(Position(6, 4));
  sequence.push_back(Position(4, 5));
  if (isValidSequence(sequence, 8)) {
    cout << "testIsValidSequence() Failure" << endl;
  }

  sequence.clear();
  sequence.push_back(Position(2, 1));
  sequence.push_back(Position(4, 2));
  sequence.push_back(Position(6, 3));
  sequence.push_back(Position(5, 5));
  sequence.push_back(Position(4, 7));
  sequence.push_back(Position(2, 6));
  sequence.push_back(Position(4, 5));
  if (isValidSequence(sequence, 8)) {
    cout << "testIsValidSequence() Success" << endl;
  }

  isValidSequence(breadthFirstSearch(17, 37, 8), 8);
  isValidSequence(breadthFirstSearch(0, 8, 3), 3);

  vector<Position> validMoves2 = getValidMoves(Position(2, 2), readMapFromFile("boardBarrierTest.txt"));


  printBoard(Position(2, 2), Position(10, 10), Position(10, 10), readMapFromFile("boardBarrierTest.txt"));
  cout << "Valid Moves:" << endl;
  for (Position position: validMoves2) {
    printBoard(Position(2, 2), Position(10, 10), position, readMapFromFile("boardBarrierTest.txt"));
  }
}