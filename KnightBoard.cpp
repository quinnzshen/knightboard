//============================================================================
// Name        : KnightBoard.cpp
// Author      : Quinn Z Shen
// Version     :
// Copyright   :
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <array>
#include <tuple>
#include <vector>
#include <stack>  
using namespace std;

template<int N>
class Board {
public:
  int grid [N][N] = {0};
  int current_move = 0;
  pair<int,int> knight_moves [8];
  const string CURRENT_POSITION = "k";

  Board() {
    knight_moves[0] = make_pair(2, 1);
    knight_moves[1] = make_pair(2, -1);
    knight_moves[2] = make_pair(1, -2);
    knight_moves[3] = make_pair(-1, -2);
    knight_moves[4] = make_pair(-2, -1);
    knight_moves[5] = make_pair(-2, 1);
    knight_moves[6] = make_pair(-1, 2);
    knight_moves[7] = make_pair(1, 2);
  }

  pair<int, int> convertPosition(string position) {
    return make_pair(position[0] - 'a', position[1] - '0' - 1);
  }

  bool are_valid_moves(int moves[], int move_length, string start_position, bool show_state) {
    reset_board();
    auto [x, y] = convertPosition(start_position);

    if (!is_valid_position(x, y)) {
      return false;
    }

    current_move = 1;
    grid[x][y] = current_move;

    if (show_state) {
      print_board();
    }

    for (int i = 0; i < move_length; i++) {
      if (!is_valid_move(x, y, moves[i])) {
        return false;
      } else {
        tie(x, y) = make_move(x, y, moves[i]);

        current_move++;

        grid[x][y] = current_move;

        if (show_state) {
          print_board();
        }
      }
    }

    return true;
  }

  void reset_board() {
    memset(grid, 0, sizeof(grid[0][0]) * N * N);
  }

  bool is_valid_position(int x, int y) {
    return ((x >= 0) && (x < N)) && ((y >= 0) && (y < N));
  }

  bool is_valid_move(int x, int y, int knight_move) {
    // Ensure move is a valid knight move
    if (knight_move > 7 || knight_move < 0) {
      return false;
    }

    int dx = get<0>(knight_moves[knight_move]);
    int dy = get<1>(knight_moves[knight_move]);

    x += dx;
    y += dy;

    return is_valid_position(x, y);
  }

  pair<int, int> make_move(int x, int y, int knight_move) {
    int dx = get<0>(knight_moves[knight_move]);
    int dy = get<1>(knight_moves[knight_move]);

    x += dx;
    y += dy;

    return make_pair(x, y);
  }

  void solve(string start_position, string goal_position) {
    reset_board();
    auto [x, y] = convertPosition(start_position);
    auto [xf, yf] = convertPosition(goal_position);

    vector<int> moves;
    stack<pair<int, int>> stack;

    stack.push(make_pair(x, y));

    while(!stack.empty()) {
      tie(x, y) = stack.pop();


      if (x == xf && y == yf) {

      }

      for (int i = 0; i < 8; i++) {
        if (is_valid_move(x, y, i)) {
          stack.push(make_move(x, y, i));
        }
      }


    }
  }

  void print_board() {
    cout << "MOVE " << current_move << ":" << endl;
    for (int y = N - 1; y >= 0; y--) {
      for (int x = 0; x < N; x++) {
        if (grid[x][y] == current_move) {
          cout << CURRENT_POSITION << " ";
        } else {
          cout << grid[x][y] << " ";
        }
      }
      cout << endl;
    }
  }

  friend ostream& operator<<(ostream &out, const Board<N> &b) {
    for (int y = 0; y < N; y++)
    {
        for (int x = 0; x < N; x++)
        {
            cout << b.grid[x][y];
        }
        out << endl;
    }
    return out;
  }
};

int main() {
  Board<8> board;

  int move_sequence[] = {1, 6, 3, 0};
  cout << boolalpha << board.are_valid_moves(move_sequence, sizeof(move_sequence) / sizeof(int), "c4", true) << endl;
}
