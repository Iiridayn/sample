#include "Queen.h"
#include "Board.h"

void Queen::getMoves(Board* board, bool usecol) {
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
  for(int i = x+1, j = y+1; i < 8 && j < 8; i++, j++) {
    if(!Piece::test(i, j, board, color, usecol))
      i = 8;
  }
  for(int i = x+1, j = y-1; i < 8 && j >= 0; i++, j--) {
    if(!Piece::test(i, j, board, color, usecol))
      i = 8;
  }
  for(int i = x-1, j = y+1; i >= 0 && j < 8; i--, j++) {
    if(!Piece::test(i, j, board, color, usecol))
      i = -1;
  }
  for(int i = x-1, j = y-1; i >= 0 && j >= 0; i--, j--) {
    if(!Piece::test(i, j, board, color, usecol))
      i = -1;
  }
  if(usecol) {
    board->rmbad(x, y);
    board->colorValid();
  }
}
