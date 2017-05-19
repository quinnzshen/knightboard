#include <iostream>
#include <algorithm>
#include <vector>
#include <iomanip>
#include <fstream>
#include <set>
#include <cassert>
using namespace std;

enum Type { WATER, ROCK, BARRIER, TELEPORT, LAVA, NORMAL };

struct Position {
  int row, col, weight;
  Type type;

  Position(int row, int col, int weight);
  Position(int row, int col, char ch = '.');

  bool isBarrier();

  friend bool operator==(Position pos1, Position pos2);
  friend bool operator!=(Position pos1, Position pos2);
  friend bool operator<(Position pos1, Position pos2);
};

const int MAX_WEIGHT = numeric_limits<int>::max();
const Position POSITION_BEGIN = Position(-1, -1, 0);

struct Move {
  Position position = POSITION_BEGIN;
  Position parentPosition = POSITION_BEGIN;
  int totalWeight;

  Move(Position current, Position parent, int weight);
  Move newMove(Position newPosition);

  friend bool operator==(Move move1, Move move2);
  friend bool operator<(Move move1, Move move2);
};

class Board {
  vector<vector<Position>> board;
  set<int> teleportPositionIds;
public:
  Board(string fileName);
  int size();

  Position positionFromId(int id);
  int idFromPosition(Position position);

  bool isValidPosition(int row, int col);
  bool isBarrierBlocking(Position start, Position end);
  bool isValidKnightMove(Position start, Position end);
  bool isValidTeleport(Position start, Position end);
  bool isValidSequence(vector<Position> sequence);

  vector<Position> getValidMoves(int row, int col);
  vector<Position> getValidMoves(Position position);

  vector<vector<Position>> getAdjacencyList();
  vector<Position> dijkstra(int startId, int goalId);

  void printBoard(Position startingPosition, Position endingPosition, Position currentPosition);
  void printBoardId();
};
