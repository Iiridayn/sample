#include "Knight.h"
#include "Board.h"

void Knight::getMoves(Board* board, bool usecol) {
  int i = x+2;
  Piece::test(i, y+1, board, color, usecol);
  Piece::test(i, y-1, board, color, usecol);
  i = x-2;
  Piece::test(i, y+1, board, color, usecol);
  Piece::test(i, y-1, board, color, usecol);
  int j = y+2;
  Piece::test(x+1, j, board, color, usecol);
  Piece::test(x-1, j, board, color, usecol);
  j = y-2;
  Piece::test(x+1, j, board, color, usecol);
  Piece::test(x-1, j, board, color, usecol);
  if(usecol) {
    board->rmbad(x, y);
    board->colorValid();
  }
}
