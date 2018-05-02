#ifndef KNIGHT_H
#define KNIGHT_H

#include "Piece.h"

class Knight : public Piece {
 public:
  Knight(int x, int y, string color):Piece(x,y,color,"knight"){}
  void getMoves(Board* board, bool color = true);
};

#endif
