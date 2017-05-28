#include "board.h"
#include <stack>
#include <thread>
#include <chrono>
using namespace std;

Board::Board(string fileName) {
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

int Board::size() {
  return board.size();
}

Position Board::positionFromId(int id) {
  int row = id / board.size();
  int col = id % board.size();

  return board[row][col];
}

int Board::idFromPosition(Position position) {
  return position.row * board.size() + position.col;
}

bool Board::isValidPosition(int row, int col) {
  if ((row >= 0) && (row < board.size()) && (col >= 0) && (col < board.size())) {
    return (board[row][col].type != ROCK) && (board[row][col].type != BARRIER);
  } else {
    return false;
  }
}

bool Board::isBarrierBlocking(Position start, Position end) {
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

bool Board::isValidKnightMove(Position start, Position end) {
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

bool Board::isValidTeleport(Position start, Position end) {
  vector<Position> potentialPositions = getValidMoves(start);
  return find(potentialPositions.begin(), potentialPositions.end(), end) != potentialPositions.end();
}

bool Board::isValidSequence(Moves path) {
  if (path.moves.empty()) {
    return true;
  }

  for (int move = 0; move < (path.moves.size() - 1); move++) {
    Position currentPosition = path.moves[move];
    Position nextPosition = path.moves[move + 1];

    if (!isValidKnightMove(currentPosition, nextPosition) && !isValidTeleport(currentPosition, nextPosition)) {
      return false;
    }
  }

  for (int move = 0; move < path.moves.size(); move++) {
    printBoard(path.moves[0], path.moves[path.moves.size() - 1], path.moves[move]);
  }

  printBoard(path.moves[0], path.moves[path.moves.size() - 1], path);

  return true;
}

bool Board::isValidSequence(vector<Position> sequence) {
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

vector<Position> Board::getValidMoves(int row, int col) {
  return getValidMoves(board[row][col]);
}

vector<Position> Board::getValidMoves(Position position) {
  vector<Position> potentialPositions;

  // Ensure that our initial position is valid
  if (!isValidPosition(position.row, position.col)) {
    return potentialPositions;
  }

  vector<pair<int, int>> deltas = {
    make_pair(2, -1), make_pair(1, -2), make_pair(-1, -2), make_pair(-2, -1),
    make_pair(-2, 1), make_pair(-1, 2), make_pair(1, 2), make_pair(2, 1)
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

vector<vector<Position>> Board::getAdjacencyList() {
  vector<vector<Position>> adjacencyList;

  for (int row = 0; row < board.size(); row++) {
    for (int col = 0; col < board.size(); col++) {
      adjacencyList.push_back(getValidMoves(board[row][col]));
    }
  }

  return adjacencyList;
}

vector<Position> Board::dijkstra(int startId, int goalId, unordered_set<int> visited) {
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
    // cout << "exploring position: " << currentMove.position.row << ", " << currentMove.position.col << " [" << currentMove.totalWeight << "]" << endl;

    pop_heap(priorityQueue.begin(), priorityQueue.end());
    priorityQueue.pop_back();

    int currentMoveId = idFromPosition(currentMove.position);

    for (Position newPosition : adjacencyList.at(currentMoveId)) {
      if (visited.find(idFromPosition(newPosition)) != visited.end()) {
        continue;
      }

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
          // cout << "backtrack: " << backtrack.row << ", " << backtrack.col << endl;
          sequence.insert(sequence.begin(), backtrack);
          backtrack = boardMoves[backtrack.row][backtrack.col].parentPosition;
          // cout << "backtrack: " << backtrack.row << ", " << backtrack.col << endl;
        }

        return sequence;
      }
    }
  }

  // cout << "No sequence found" << endl;
  return vector<Position>();
}

Moves Board::longestPath(int startId, int goalId) {
  vector<vector<Position>> adjacencyList = getAdjacencyList();
  stack<Moves> stack;
  Moves solution;
  int longestPathWeight = -1;
  int count = 0;

  stack.push(Moves(positionFromId(startId), startId));

  while (!stack.empty()) {
    count++;
    Moves currentPath = stack.top();
    stack.pop();

    Position currentPosition = currentPath.moves.back();
    int currentId = idFromPosition(currentPosition);

    printBoard(positionFromId(startId), positionFromId(goalId), currentPath);
    this_thread::sleep_for (chrono::milliseconds(100));

    // cout << "Currently at: " << currentPosition.row << ", " << currentPosition.col << " [" << currentPath.totalWeight << "]" << endl;
    // cout << stack.size() << endl;

    if (currentPosition == positionFromId(goalId)) {
      if (currentPath.totalWeight > longestPathWeight) {
        longestPathWeight = currentPath.totalWeight;
        solution = currentPath;
        isValidSequence(currentPath);
        cout << "Found a solution: " << longestPathWeight << endl;
      }
    } else {
      vector<Position> explorePositions = adjacencyList[currentId];

      // Randomly shuffle the explore positions
      rotate(explorePositions.begin(), explorePositions.begin() + (count % explorePositions.size()), explorePositions.end());

      for (Position explorePosition : explorePositions) {
        // Check if we can still reach current location from our goal
        Moves explorePath = currentPath;
        // cout << "Exploring: " << explorePosition.row << ", " << explorePosition.col << " [" << explorePath.totalWeight << "]" << " (" << explorePath.visited.size() << ")" << endl;

        if (explorePath.newMove(explorePosition, idFromPosition(explorePosition))) {
          // cout << "visited: [ ";
          // for (int x : currentPath.visited) {
          //   cout << x << " ";
          // }
          // cout << "]" << endl;

          if (dijkstra(goalId, idFromPosition(explorePosition), currentPath.visited).size() == 0 && (goalId != idFromPosition(explorePosition))) {
            // cout << "Can't reach goal, not adding." << endl;
            // cout << "uhh" << endl;
            continue;
          }

          // cout << "Adding to stack: " << explorePosition.row << ", " << explorePosition.col << " [" << explorePath.totalWeight << "]" << endl;
          stack.push(explorePath);
        }
      }
    }
  }

  if (longestPathWeight == -1) {
    cout << "Couldn't find a path." << endl;
  }

  return solution;
}

void Board::printBoard(Position startingPosition, Position endingPosition, Moves path) {
  Position currentPosition = path.moves.empty() ? POSITION_BEGIN : path.moves.back();

  cout << "Board[" << path.totalWeight << "]:" << endl;
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
      } else if (path.visited.find(idFromPosition(tile)) != path.visited.end()) {
        cout << "# ";
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

void Board::printBoard(Position startingPosition, Position endingPosition, Position currentPosition) {
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

void Board::printBoardId() {
  int maxDigits = ((board.size() * board.size()) % 10) + 1;
  int id = 0;
  for (int row = 0; row < board.size(); row++) {
    for (int col = 0; col < board.size(); col++) {
      cout << setw(maxDigits) << id++ << " ";
    }
    cout << endl;
  }
}
