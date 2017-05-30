#include "board.h"
using namespace std;

const int DEFAULT_EXPLORE_DEPTH = 1200;
const int DEFAULT_SLEEP_MS = 100;

/* main.cpp allows us to run the knightboard challenge */
int main() {
  // Level 3
  Board board = Board("boards/lvl3Board.txt");
  board.isValidSequence(board.dijkstra(17, 37));

  cout << "Completed level 3. Press Enter to Continue:" << endl;
  cin.ignore();

  // Level 4
  board = Board("boards/board.txt");
  board.isValidSequence(board.dijkstra(13, 856));

  cout << "Completed level 4. Press Enter to Continue:" << endl;
  cin.ignore();

  // Level 5
  board.isValidSequence(board.longestPath(13, 856, DEFAULT_EXPLORE_DEPTH, DEFAULT_SLEEP_MS));

  cout << "Completed level 5." << endl;
}
