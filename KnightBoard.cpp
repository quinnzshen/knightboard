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

const Position MOVE_START = Position(-1, -1, '.');

struct Move {
  Position position = MOVE_START;
  Position parentPosition = MOVE_START;
  int totalWeight;

  Move(Position current, Position parent, int weight) {
    position = current;
    parentPosition = parent;
    totalWeight = weight;
  }

  friend bool operator==(Move move1, Move move2) {
    return move1.position == move2.position;
  }

  friend bool operator<(Move move1, Move move2) {
    return move1.totalWeight > move2.totalWeight;
  }
};

class Sequence {
  vector<Position> sequence;
  public:int weight;

  public:Sequence() {
    weight = 0;
  }

  public:Sequence(Position position) {
    sequence.push_back(position);
    weight = position.weight;
  }

  void add(Position position) {
    sequence.push_back(position);
    weight += position.weight;
  }

  Position get(int index) {
    return sequence[index];
  }

  bool empty() {
    return sequence.empty();
  }

  int size() {
    return sequence.size();
  }

  friend bool operator<(Sequence seq1, Sequence seq2) {
    return seq1.weight > seq2.weight;
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

  bool isValidKnightMove(Position start, Position end) {
    // Ensure that starting and ending positions are both valid
    if (!isValidPosition(start.row, start.col) || !isValidPosition(end.row, end.col)) {
      return false;
    }

    // Determine if it's possible to teleport to the end location.
    if (end.type == TELEPORT) {
      for (Position option : getValidMoves(start)) {
        if (option == end) {
          return true;
        }
      }
      return false;
    }

    // Otherwise, determine if the move was a valid knight jump.
    int deltaRow = end.row - start.row;
    int deltaCol = end.col - start.col;

    // Ensure that the move is a valid knight move
    if (!((abs(deltaRow) == 1 && abs(deltaCol) == 2) || (abs(deltaRow) == 2 && abs(deltaCol) == 1))) {
      return false;
    }

    if (deltaCol == 2 && (board[start.row][start.col + 1].isBarrier() || board[start.row][start.col + 2].isBarrier())) {
      return false;
    } else if (deltaCol == -2 && (board[start.row][start.col - 1].isBarrier() || board[start.row][start.col - 2].isBarrier())) {
      return false;
    } else if (deltaRow == 2 && (board[start.row + 1][start.col].isBarrier() || board[start.row + 2][start.col].isBarrier())) {
      return false;
    } else if (deltaRow == -2 && (board[start.row - 1][start.col].isBarrier() || board[start.row - 2][start.col].isBarrier())) {
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
      // Ensure that the valid move's type is not a rock or barrier.
      if (move.type == BARRIER || move.type == ROCK) {
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
      } else if (isValidKnightMove(position, move)) {
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
    vector<vector<Move>> boardMoves (board.size(), vector<Move> (board.size(), Move(MOVE_START, MOVE_START, MAX_WEIGHT)));

    vector<Move> priorityQueue;
    Position startPosition = positionFromId(startId);
    boardMoves[startPosition.row][startPosition.col] = Move(startPosition, MOVE_START, 0);

    priorityQueue.push_back(Move(startPosition, MOVE_START, startPosition.weight));
    push_heap(priorityQueue.begin(), priorityQueue.end());

    while (!priorityQueue.empty()) {
      Move currentMove = priorityQueue.front();
      cout << "exploring position: " << currentMove.position.row << ", " << currentMove.position.col << " [" << currentMove.totalWeight << "]" << endl;

      pop_heap(priorityQueue.begin(), priorityQueue.end());
      priorityQueue.pop_back();

      int currentMoveId = idFromPosition(currentMove.position);

      for (Position newPosition : adjacencyList.at(currentMoveId)) {
        Move newMove = Move(newPosition, currentMove.position, currentMove.totalWeight + newPosition.weight);
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

          while (backtrack != MOVE_START) {
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

  bool isValidSequence(vector<Position> sequence) {
    if (sequence.empty()) {
      return true;
    }

    for (int move = 0; move < (sequence.size() - 1); move++) {
      cout << "Checking validity of: " << sequence[move].row << ", " << sequence[move].col << endl;
      if (!isValidKnightMove(sequence[move], sequence[move + 1])) {
        cout << "Failed validity." << endl;
        return false;
      }
    }

    for (int move = 0; move < sequence.size(); move++) {
      printBoard(sequence[0], sequence[sequence.size() - 1], sequence[move]);
    }

    return true;
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
  if (sequence.empty()
) {
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

  printBoardId(5);

  // cout << "Valid Moves:" << endl;
  // for (Position position: board.getValidMoves(Position(2, 2))) {
  //   board.printBoard(Position(2, 2), Position(10, 10), position);
  // }

  board = Board("boardTeleTest.txt");

  // Testing getAdjacencyList
  // vector<vector<Position>> adjacencyList2 = board.getAdjacencyList();
  // for (int nodeId = 0; nodeId < board.size() * board.size(); nodeId++) {
  //   cout << nodeId << ": ";
  //   vector<Position> node = adjacencyList2.at(nodeId);
  //   for (Position position : node) {
  //     cout << board.idFromPosition(position) << "(" << position.weight << ") ";
  //   }
  //   cout << endl;
  // }

  board.printBoard(Position(-1, -1), Position(-1, -1), Position(-1, -1));
  cout << boolalpha << board.isValidSequence(board.dijkstra(0, 99)) << endl;

  // board = Board("board.txt");
  // cout << boolalpha << board.isValidSequence(board.dijkstra(835, 13)) << endl;

  // priority_queue<Sequence> queue;

  //
  // Sequence sequence2;
  // sequence2.add(Position(2, 1, 'L'));
  // sequence2.add(Position(4, 2, 'L'));
  //
  // Sequence sequence1;
  // sequence1.add(Position(2, 1, '.'));
  // sequence1.add(Position(4, 2, '.'));
  // sequence1.add(Position(6, 3, '.'));
  // queue.push(sequence1);
  // sequence1.add(Position(5, 5, '.'));
  // sequence1.add(Position(4, 7, '.'));
  // sequence1.add(Position(2, 6, '.'));
  // sequence1.add(Position(4, 5, '.'));
  // queue.push(sequence1);
  // queue.push(sequence2);

  // cout << queue.top().weight << endl;
  // queue.pop();
  // cout << queue.top().weight << endl;
  //
  // cout << boolalpha << board.isValidSequence(sequence1) << endl;
  // cout << sequence1.weight << endl;

  // queue.push(Position(1, 1, '.'));
  // queue.push(Position(2, 2, 'L'));
  // queue.push(Position(1, 2, '.'));
  //
  // cout << queue.top().row << ", " << queue.top().col << endl;
  // queue.pop();
  // cout << queue.top().row << ", " << queue.top().col << endl;
  // queue.pop();
  // cout << queue.top().row << ", " << queue.top().col << endl;

}
