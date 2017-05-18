//============================================================================
// Name        : KnightBoard.cpp
// Author      : Quinn Z Shen
// Version     :
// Copyright   :
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <iomanip>
#include <fstream>
#include <set>
#include <cassert>
using namespace std;

enum Type { WATER, ROCK, BARRIER, TELEPORT, LAVA, NORMAL };
const int MAX_WEIGHT = numeric_limits<int>::max();

struct Position {
  int row, col, weight;
  Type type;

  Position(int row, int col, int weight) {
    this->row = row;
    this->col = col;
    this->weight = weight;
  }

  Position(int row, int col, char ch = '.') {
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

  bool isBarrier() {
    return type == BARRIER;
  }

  friend bool operator==(Position pos1, Position pos2) {
    return (pos1.row == pos2.row) && (pos1.col == pos2.col);
  }

  friend bool operator!=(Position pos1, Position pos2) {
    return (pos1.row != pos2.row) || (pos1.col != pos2.col);
  }

  friend bool operator<(Position pos1, Position pos2) {
    return pos1.weight > pos2.weight;
  }
};

const Position POSITION_BEGIN = Position(-1, -1, 0);

struct Move {
  Position position = POSITION_BEGIN;
  Position parentPosition = POSITION_BEGIN;
  int totalWeight;

  Move(Position current, Position parent, int weight) {
    position = current;
    parentPosition = parent;
    totalWeight = weight;
  }

  Move newMove(Position newPosition) {
    return Move(newPosition, position, totalWeight + newPosition.weight);
  }

  friend bool operator==(Move move1, Move move2) {
    return move1.position == move2.position;
  }

  friend bool operator<(Move move1, Move move2) {
    return move1.totalWeight > move2.totalWeight;
  }
};

class Board {
  vector<vector<Position>> board;
  set<int> teleportPositionIds;

  public:Board(string fileName) {
    char ch;
    int row = 0;
    int col = 0;
    int id = 0;

    fstream infile(fileName);

    while (infile >> noskipws >> ch) {
      if (ch == '\n') {
        row++;
        col = 0;
        continue;
      }

      if (ch == 'T') {
        teleportPositionIds.insert(id);
      }

      if (col == 0) {
        vector<Position> vectorRow;
        board.push_back(vectorRow);
      }

      board[row].push_back(Position(row, col, ch));
      col++;
      id++;
    }

    // Ensure that the board is a square
    assert(board.size() == board[0].size());
  }

  Position positionFromId(int id) {
    int row = id / board.size();
    int col = id % board.size();

    return board[row][col];
  }

  int idFromPosition(Position position) {
    return position.row * board.size() + position.col;
  }

  int size() {
    return board.size();
  }

  bool isValidPosition(int row, int col) {
    if ((row >= 0) && (row < board.size()) && (col >= 0) && (col < board.size())) {
      return (board[row][col].type != ROCK) && (board[row][col].type != BARRIER);
    } else {
      return false;
    }
  }

  bool isBarrierBlocking(Position start, Position end) {
    int deltaRow = end.row - start.row;
    int deltaCol = end.col - start.col;

    // Ensure that the move is a valid knight move
    assert((abs(deltaRow) == 1 && abs(deltaCol) == 2) || (abs(deltaRow) == 2 && abs(deltaCol) == 1));

    if (deltaCol == 2 && (board[start.row][start.col + 1].isBarrier() || board[start.row][start.col + 2].isBarrier())) {
      return true;
    } else if (deltaCol == -2 && (board[start.row][start.col - 1].isBarrier() || board[start.row][start.col - 2].isBarrier())) {
      return true;
    } else if (deltaRow == 2 && (board[start.row + 1][start.col].isBarrier() || board[start.row + 2][start.col].isBarrier())) {
      return true;
    } else if (deltaRow == -2 && (board[start.row - 1][start.col].isBarrier() || board[start.row - 2][start.col].isBarrier())) {
      return true;
    }

    return false;
  }

  bool isValidKnightMove(Position start, Position end) {
    // Ensure that starting and ending positions are both valid
    if (!isValidPosition(start.row, start.col) || !isValidPosition(end.row, end.col)) {
      return false;
    }

    // Otherwise, determine if the move was a valid knight jump.
    int deltaRow = end.row - start.row;
    int deltaCol = end.col - start.col;

    // Ensure that the move is a valid knight move
    if(!((abs(deltaRow) == 1 && abs(deltaCol) == 2) || (abs(deltaRow) == 2 && abs(deltaCol) == 1))) {
      return false;
    }

    // Otherwise determine if there are barriers blocking the move
    if(isBarrierBlocking(start, end)) {
      return false;
    }

    return true;
  }

  vector<Position> getValidMoves(int row, int col) {
    return getValidMoves(board[row][col]);
  }

  vector<Position> getValidMoves(Position position) {
    vector<Position> potentialPositions;

    // Ensure that our initial position is valid
    if (!isValidPosition(position.row, position.col)) {
      return potentialPositions;
    }

    vector<pair<int, int>> deltas = {
      make_pair(2, -1), make_pair(2, 1), make_pair(-2, -1), make_pair(-2, 1),
      make_pair(1, 2), make_pair(1, -2), make_pair(-1, 2), make_pair(-1, -2)
    };

    // Ensure that our attempted move doesn't land on a barrier/rock or out of bounds
    for (pair<int, int> delta : deltas) {
      auto [deltaRow, deltaCol] = delta;
      if (isValidPosition(position.row + deltaRow, position.col + deltaCol)) {
        potentialPositions.push_back(board[position.row + deltaRow][position.col + deltaCol]);
      }
    }

    vector<Position> validMoves;
    for (Position move: potentialPositions) {
      // Ensure there are no barriers blocking the move
      if (isBarrierBlocking(position, move)) {
        continue;
      }

      int moveId = idFromPosition(move);

      // If the move ends on a teleport type, find valid teleport locations.
      if (teleportPositionIds.find(moveId) != teleportPositionIds.end()) {
        for (int teleportPositionId : teleportPositionIds) {
          if (moveId != teleportPositionId) {
            validMoves.push_back(positionFromId(teleportPositionId));
          }
        }
      } else {
        validMoves.push_back(board[move.row][move.col]);
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

  vector<vector<Position>> getAdjacencyList() {
    vector<vector<Position>> adjacencyList;

    for (int row = 0; row < board.size(); row++) {
      for (int col = 0; col < board.size(); col++) {
        adjacencyList.push_back(getValidMoves(board[row][col]));
      }
    }

    return adjacencyList;
  }

  vector<Position> dijkstra(int startId, int goalId) {
    vector<vector<Position>> adjacencyList = getAdjacencyList();
    vector<vector<Move>> boardMoves (board.size(), vector<Move> (board.size(), Move(POSITION_BEGIN, POSITION_BEGIN, MAX_WEIGHT)));
    vector<Move> priorityQueue;

    Position startPosition = positionFromId(startId);
    Move startingMove = Move(startPosition, POSITION_BEGIN, startPosition.weight);

    boardMoves[startPosition.row][startPosition.col] = startingMove;
    priorityQueue.push_back(startingMove);
    push_heap(priorityQueue.begin(), priorityQueue.end());

    while (!priorityQueue.empty()) {
      Move currentMove = priorityQueue.front();
      cout << "exploring position: " << currentMove.position.row << ", " << currentMove.position.col << " [" << currentMove.totalWeight << "]" << endl;

      pop_heap(priorityQueue.begin(), priorityQueue.end());
      priorityQueue.pop_back();

      int currentMoveId = idFromPosition(currentMove.position);

      for (Position newPosition : adjacencyList.at(currentMoveId)) {
        Move newMove = currentMove.newMove(newPosition);
        int newPositionId = idFromPosition(newPosition);

        if (newMove.totalWeight < boardMoves[newPosition.row][newPosition.col].totalWeight) {
          boardMoves[newPosition.row][newPosition.col] = newMove;

          vector<Move>::iterator itr = find(priorityQueue.begin(), priorityQueue.end(), newMove);
          if (itr != priorityQueue.end()) {
            priorityQueue.erase(itr);
            make_heap(priorityQueue.begin(), priorityQueue.end());
          }

          priorityQueue.push_back(newMove);
          push_heap(priorityQueue.begin(), priorityQueue.end());
        }

        if (newPositionId == goalId) {
          // Once we've reached the goal, return the move sequence.
          vector<Position> sequence;
          Position backtrack = newPosition;

          while (backtrack != POSITION_BEGIN) {
            cout << "backtrack: " << backtrack.row << ", " << backtrack.col << endl;
            sequence.insert(sequence.begin(), backtrack);
            backtrack = boardMoves[backtrack.row][backtrack.col].parentPosition;
            cout << "backtrack: " << backtrack.row << ", " << backtrack.col << endl;
          }

          return sequence;
        }
      }
    }

    cout << "No sequence found" << endl;
    return vector<Position>();
  }

  bool isValidTeleport(Position start, Position end) {
    vector<Position> potentialPositions = getValidMoves(start);
    return find(potentialPositions.begin(), potentialPositions.end(), end) != potentialPositions.end();
  }

  bool isValidSequence(vector<Position> sequence) {
    if (sequence.empty()) {
      return true;
    }

    for (int move = 0; move < (sequence.size() - 1); move++) {
      Position currentPosition = sequence[move];
      Position nextPosition = sequence[move + 1];

      cout << "Checking validity of: " << currentPosition.row << ", " << currentPosition.col << endl;


      if (!isValidKnightMove(currentPosition, nextPosition) && !isValidTeleport(currentPosition, nextPosition)) {
        cout << "Failed validity." << endl;
        return false;
      }
    }

    for (int move = 0; move < sequence.size(); move++) {
      printBoard(sequence[0], sequence[sequence.size() - 1], sequence[move]);
    }

    return true;
  }

  void printBoardId() {
    int maxDigits = ((board.size() * board.size()) % 10) + 1;
    int id = 0;
    for (int row = 0; row < board.size(); row++) {
      for (int col = 0; col < board.size(); col++) {
        cout << setw(maxDigits) << id++ << " ";
      }
      cout << endl;
    }
  }
};

int main() {
  Board testBoard = Board("lvl3Board.txt");

  // Testing positionFromId
  Position position = testBoard.positionFromId(4);
  assert(position.row == 0 && position.col == 4);

  // Testing idFromPosition
  int id = testBoard.idFromPosition(Position(1, 1));
  assert(id == 9);

  // Testing isValidPosition
  bool validPosition = testBoard.isValidPosition(1, 1);
  assert(validPosition);

  // Testing isValidPosition (out of board)
  validPosition = testBoard.isValidPosition(9, 1);
  assert(!validPosition);

  // Testing isValidKnightMove
  bool validKnightMove = testBoard.isValidKnightMove(Position(0, 0), Position(1, 2));
  assert(validKnightMove);

  // Testing isValidKnightMove (invalid knight jump)
  validKnightMove = testBoard.isValidKnightMove(Position(0, 0), Position(1, 1));
  assert(!validKnightMove);

  // Testing isValidKnightMove (valid knight jump is off the board)
  validKnightMove = testBoard.isValidKnightMove(Position(0, 0), Position(-1, -2));
  assert(!validKnightMove);

  // Testing getValidMoves
  vector<Position> validMoves = testBoard.getValidMoves(Position(0, 0));
  for (Position move : validMoves) {
    if (!((move.row == 2 && move.col == 1) || (move.row == 1 && move.col == 2))) {
      cout << "testGetValidMoves Failure" << endl;
    }
  }

  // Testing createAdjacencyList
  vector<vector<Position>> adjacencyList = testBoard.getAdjacencyList();
  for (int nodeId = 0; nodeId < 8 * 8; nodeId++) {
    cout << nodeId << ": ";
    vector<Position> node = adjacencyList.at(nodeId);
    for (Position position : node) {
      cout << testBoard.idFromPosition(position) << " ";
    }
    cout << endl;
  }

  testBoard.printBoardId();

  // Testing isValidSequence
  vector<Position> sequence;
  sequence.push_back(Position(2, 1));
  sequence.push_back(Position(4, 2));
  sequence.push_back(Position(6, 4));
  sequence.push_back(Position(4, 5));
  assert(!testBoard.isValidSequence(sequence));

  sequence.clear();
  sequence.push_back(Position(2, 1));
  sequence.push_back(Position(4, 2));
  sequence.push_back(Position(6, 3));
  sequence.push_back(Position(5, 5));
  sequence.push_back(Position(4, 7));
  sequence.push_back(Position(2, 6));
  sequence.push_back(Position(4, 5));
  assert(testBoard.isValidSequence(sequence));

  // Level 3
  testBoard.isValidSequence(testBoard.dijkstra(17, 37));

  Board board = Board("lvl4BoardTest.txt");
  validMoves = board.getValidMoves(2, 2);

  cout << validMoves.size() << endl;
  assert(validMoves.size() == 5);
  // Valid Knight Jumps
  assert(find(validMoves.begin(), validMoves.end(), Position(1, 0)) != validMoves.end());
  assert(find(validMoves.begin(), validMoves.end(), Position(0, 1)) != validMoves.end());
  assert(find(validMoves.begin(), validMoves.end(), Position(3, 0)) != validMoves.end());
  // Valid Teleport Positions
  assert(find(validMoves.begin(), validMoves.end(), Position(4, 0)) != validMoves.end());
  assert(find(validMoves.begin(), validMoves.end(), Position(1, 4)) != validMoves.end());

  board = Board("boardTeleTest.txt");

  // Testing getAdjacencyList w/ weights on complex board
  board = Board("testAdjacencyListBoard.txt");
  adjacencyList = board.getAdjacencyList();
  assert(adjacencyList[0].size() == 0);
  assert(adjacencyList[1].size() == 1);
  assert(adjacencyList[1][0] == Position(2, 2));
  assert(adjacencyList[1][0].weight == 2);
  assert(adjacencyList[2].size() == 2);
  assert(adjacencyList[2][0] == Position(2, 1));
  assert(adjacencyList[2][0].weight == 4);
  assert(adjacencyList[2][1] == Position(1, 0));
  assert(adjacencyList[2][1].weight == 1);
  assert(adjacencyList[3].size() == 2);
  assert(adjacencyList[3][0] == Position(2, 2));
  assert(adjacencyList[3][0].weight == 2);
  assert(adjacencyList[3][1] == Position(1, 1));
  assert(adjacencyList[3][1].weight == 1);
  assert(adjacencyList[4].size() == 0);
  assert(adjacencyList[5].size() == 0);
  assert(adjacencyList[6].size() == 0);
  assert(adjacencyList[7].size() == 1);
  assert(adjacencyList[7][0] == Position(1, 1));
  assert(adjacencyList[7][0].weight == 1);
  assert(adjacencyList[8].size() == 1);
  assert(adjacencyList[8][0] == Position(0, 1));
  assert(adjacencyList[8][0].weight == 1);

  board.printBoard(Position(-1, -1), Position(-1, -1), Position(-1, -1));
  board.isValidSequence(board.dijkstra(0, 99));

  board = Board("board.txt");
  board.isValidSequence(board.dijkstra(13, 856));
}
