#ifndef KING_H
#define KING_H

#include "Piece.h"

class King : public Piece {
 public:
  King(int x, int y, string color):Piece(x,y,color,"king"){}
  void getMoves(Board* board, bool color = true);
};

#endif
