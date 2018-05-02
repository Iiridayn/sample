#ifndef ROOK_H
#define ROOK_H

#include "Piece.h"

class Rook : public Piece {
 public:
  Rook(int x, int y, string color):Piece(x,y,color,"rook"){}
  void getMoves(Board* board, bool color = true);
};

#endif
