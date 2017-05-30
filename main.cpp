#include "board.h"
#include <stack>
using namespace std;

int main() {
  unordered_set<int> visited;
  visited.insert(27);

  // Level 3
  // Board board = Board("boards/lvl3Board.txt");
  // board.isValidSequence(board.dijkstra(17, 37, visited));

  // Level 4
  Board board = Board("boards/board.txt");
  // board.isValidSequence(board.dijkstra(13, 856));

  // Level 5
  // board = Board("boards/4x4Board.txt");
  // board = Board("boards/lvl3Board.txt");
  board.isValidSequence(board.longestPath(411, 404));
}
