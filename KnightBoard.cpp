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
#include <set>
#include <cassert>
using namespace std;

enum Type { WATER, ROCK, BARRIER, TELEPORT, LAVA, NORMAL };

struct Position {
  int row, col;
  Type type;

  Position(int row, int col, char ch = '.') {
    this->row = row;
    this->col = col;

    switch(ch) {
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

  bool isBarrier() {
    return this->type == BARRIER;
  }

  friend bool operator==(Position pos1, Position pos2) {
    return (pos1.row == pos2.row) && (pos1.col == pos2.col);
  }
};

class Board {
  vector<vector<Position>> board;
  // set<Position> teleportPositions;

  public:Board(string fileName) {
    char ch;
    int row = 0;
    int col = 0;

    fstream infile(fileName);

    while (infile >> noskipws >> ch) {
      if (ch == '\n') {
        row++;
        col = 0;
        continue;
      }

      if (ch == 'T') {
        // teleportPositions.insert(Position(row, col, ch));
      }

      if (col == 0) {
        vector<Position> vectorRow;
        board.push_back(vectorRow);
      }

      board[row].push_back(Position(row, col, ch));
      col++;
    }

    // Ensure that the board is a square
    assert(board.size() == board[0].size());
  }

  Position get(int row, int col) {
    return board[row][col];
  }

  Position get(Position position) {
    return board[position.row][position.col];
  }

  int size() {
    return board.size();
  }

  bool isValidPosition(Position position) {
    Position boardPosition = this->get(position);

    return (boardPosition.row >= 0) && (boardPosition.row < this->size()) && 
    (boardPosition.col >= 0) && (boardPosition.col < this->size()) && 
    (boardPosition.type != ROCK) && (boardPosition.type != BARRIER);
  }

  bool isValidKnightMove(Position start, Position end) {
    // Ensure that starting and ending positions are both valid
    if (!this->isValidPosition(start) || !this->isValidPosition(end)) {
      return false;
    }

    int deltaRow = end.row - start.row;
    int deltaCol = end.col - start.col;

    // Ensure that the move is a valid knight move
    if (!((abs(deltaRow) == 1 && abs(deltaCol) == 2) || (abs(deltaRow) == 2 && abs(deltaCol) == 1))) {
      return false;
    }

    if (deltaCol == 2 && (this->get(start.row, start.col + 1).isBarrier() || this->get(start.row, start.col + 2).isBarrier())) {
      return false;
    } else if (deltaCol == -2 && (this->get(start.row, start.col - 1).isBarrier() || this->get(start.row, start.col - 2).isBarrier())) {
      return false;
    } else if (deltaRow == 2 && (this->get(start.row + 1, start.col).isBarrier() || this->get(start.row + 2, start.col).isBarrier())) {
      return false;
    } else if (deltaRow == -2 && (this->get(start.row - 1, start.col).isBarrier() || this->get(start.row - 2, start.col).isBarrier())) {
      return false;
    }

    return true;
  }

  vector<Position> getValidMoves(Position position) {
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
      if (this->isValidKnightMove(position, move)) {
        validMoves.push_back(move);
      }
    }

    return validMoves;
  }

  void printBoard(Position startingPosition, Position endingPosition, Position currentPosition) {
    cout << "Board:" << endl;
    for (vector<Position> row : board) {
      for (Position tile : row) {
        if ((startingPosition == currentPosition || endingPosition == currentPosition) && tile == currentPosition) {
          cout << "* ";
        } else if (tile == startingPosition) {
          cout << "S ";
        } else if (tile == endingPosition) {
          cout << "E ";
        } else if (tile == currentPosition) {
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

  Board board = Board("boardBarrierTest.txt");
  board.printBoard(Position(2, 2), Position(10, 10), Position(10, 10));

  cout << "Valid Moves:" << endl;
  for (Position position: board.getValidMoves(Position(2, 2))) {
    board.printBoard(Position(2, 2), Position(10, 10), position);
  }
}