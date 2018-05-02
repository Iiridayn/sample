#ifndef PAWN_H
#define PAWN_H

#include "Piece.h"

class Pawn : public Piece {
 public:
  Pawn(int x, int y, string color):Piece(x,y,color,"pawn"){}
  void getMoves(Board* board, bool color = true);
};

#endif
