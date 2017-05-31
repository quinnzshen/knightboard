#include "board.h"
using namespace std;

/* test.cpp is used for unit-testing sections of code */
int main() {
  Board board = Board("boards/lvl3Board.txt");

  // Testing positionFromId
  Position position = board.positionFromId(4);
  assert(position.row == 0 && position.col == 4);

  // Testing idFromPosition
  int id = board.idFromPosition(Position(1, 1));
  assert(id == 9);

  // Testing isValidPosition
  bool validPosition = board.isValidPosition(1, 1);
  assert(validPosition);

  // Testing isValidPosition (out of board)
  validPosition = board.isValidPosition(9, 1);
  assert(!validPosition);

  // Testing isValidKnightMove
  bool validKnightMove = board.isValidKnightMove(Position(0, 0), Position(1, 2));
  assert(validKnightMove);

  // Testing isValidKnightMove (invalid knight jump)
  validKnightMove = board.isValidKnightMove(Position(0, 0), Position(1, 1));
  assert(!validKnightMove);

  // Testing isValidKnightMove (valid knight jump is off the board)
  validKnightMove = board.isValidKnightMove(Position(0, 0), Position(-1, -2));
  assert(!validKnightMove);

  // Testing getValidPath
  vector<Position> validMoves = board.getValidPath(Position(0, 0));
  for (Position move : validMoves) {
    assert((move.row == 2 && move.col == 1) || (move.row == 1 && move.col == 2));
  }

  // Testing isValidSequence
  vector<Position> sequence;
  sequence.push_back(Position(2, 1));
  sequence.push_back(Position(4, 2));
  sequence.push_back(Position(6, 4));
  sequence.push_back(Position(4, 5));
  assert(!board.isValidSequence(sequence));

  sequence.clear();
  sequence.push_back(Position(2, 1));
  sequence.push_back(Position(4, 2));
  sequence.push_back(Position(6, 3));
  sequence.push_back(Position(5, 5));
  sequence.push_back(Position(4, 7));
  sequence.push_back(Position(2, 6));
  sequence.push_back(Position(4, 5));
  assert(board.isValidSequence(sequence));

  // Level 4 Unit Tests
  board = Board("boards/lvl4BoardTest.txt");
  validMoves = board.getValidPath(2, 2);

  assert(validMoves.size() == 5);
  // Valid Knight Jumps
  assert(find(validMoves.begin(), validMoves.end(), Position(1, 0)) != validMoves.end());
  assert(find(validMoves.begin(), validMoves.end(), Position(0, 1)) != validMoves.end());
  assert(find(validMoves.begin(), validMoves.end(), Position(3, 0)) != validMoves.end());
  // Valid Teleport Positions
  assert(find(validMoves.begin(), validMoves.end(), Position(4, 0)) != validMoves.end());
  assert(find(validMoves.begin(), validMoves.end(), Position(1, 4)) != validMoves.end());

  // Testing getAdjacencyList w/ weights on complex board
  board = Board("boards/testAdjacencyListBoard.txt");
  vector<vector<Position>> adjacencyList = board.getAdjacencyList();
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
  assert(adjacencyList[3][0] == Position(1, 1));
  assert(adjacencyList[3][0].weight == 1);
  assert(adjacencyList[3][1] == Position(2, 2));
  assert(adjacencyList[3][1].weight == 2);
  assert(adjacencyList[4].size() == 0);
  assert(adjacencyList[5].size() == 0);
  assert(adjacencyList[6].size() == 0);
  assert(adjacencyList[7].size() == 1);
  assert(adjacencyList[7][0] == Position(1, 1));
  assert(adjacencyList[7][0].weight == 1);
  assert(adjacencyList[8].size() == 1);
  assert(adjacencyList[8][0] == Position(0, 1));
  assert(adjacencyList[8][0].weight == 1);

  // Testing dijkstra algorithm
  board = Board("boards/boardTeleTest.txt");
  Path solution = board.dijkstra(0, 95);
  assert(solution.moves.size() == 3);
  assert(solution.moves[0] == Position(0, 0));
  assert(solution.moves[1] == Position(8, 7));
  assert(solution.moves[2] == Position(9, 5));
}
