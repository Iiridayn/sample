#ifndef QUEEN_H
#define QUEEN_H

#include "Piece.h"

class Queen : public Piece {
 public:
  Queen(int x, int y, string color):Piece(x,y,color,"queen"){}
  void getMoves(Board* board, bool color = true);
};

#endif
