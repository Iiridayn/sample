#ifndef BISHOP_H
#define BISHOP_H

#include "Piece.h"

class Bishop : public Piece {
 public:
  Bishop(int x, int y, string color):Piece(x,y,color,"bishop"){}
  void getMoves(Board* board, bool color = true);
};

#endif
