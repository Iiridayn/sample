#include "Rook.h"
#include "Board.h"

void Rook::getMoves(Board* board, bool usecol) {
  for(int i = x+1; i < 8; i++) {
    if(!Piece::test(i, y, board, color, usecol))
      i = 8;
  }
  for(int i = x-1; i >= 0; i--) {
    if(!Piece::test(i, y, board, color, usecol))
      i = -1;
  }
  for(int j = y+1; j < 8; j++) {
    if(!Piece::test(x, j, board, color, usecol))
      j = 8;
  }
  for(int j = y-1; j >=0; j--) {
    if(!Piece::test(x, j, board, color, usecol))
      j = -1;
  }
  if(usecol) {
    board->rmbad(x, y);
    board->colorValid();
  }
}
