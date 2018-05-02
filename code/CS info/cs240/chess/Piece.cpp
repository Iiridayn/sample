#include "Piece.h"
#include "Board.h"

bool Piece::test(int i, int j, Board* board, string incolor, bool color) {
  if(i < 0 || i >= 8 || j < 0 || j >= 8) return false;
  bool ret = false;
  //cout << "test: " << i << ", " << j << endl;
  if(board->board[i][j] != 0) {
    //cout << "contains: " << board->board[i][j]->type << endl;
    if(board->board[i][j]->color != incolor) {
      board->poss.insert((i*10)+j);
      //cout << "adding: " << ((i*10)+j) << endl;
    }
  } //how know is good move???
  else {
    board->poss.insert((i*10)+j);
    //cout << "adding: " << ((i*10)+j) << endl;
    ret = true;
  }
  //cout << "ret: " << (ret?"true":"false") << endl;
  return ret;
}
