#include "King.h"
#include "Board.h"

void King::getMoves(Board* board, bool usecol) {
  for(int i = x-1; i < x+2; i++)
    for(int j = y-1; j < y+2; j++)
      Piece::test(i, j, board, color, usecol);
  if(usecol) {
    board->rmbad(x, y);
    board->colorValid();
  }
}
