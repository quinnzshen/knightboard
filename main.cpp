#include "board.h"
using namespace std;

const int DEFAULT_EXPLORE_DEPTH = 1200;

int main() {
  // Level 3
  // Board board = Board("boards/lvl3Board.txt");
  // board.isValidSequence(board.dijkstra(17, 37, visited));

  // Level 4
  Board board = Board("boards/board.txt");
  // board.isValidSequence(board.dijkstra(13, 856));

  // Level 5
  // board = Board("boards/4x4Board.txt");
  // board = Board("boards/lvl3Board.txt");
  board.isValidSequence(board.longestPath(411, 404, DEFAULT_EXPLORE_DEPTH));
}
