#ifndef PIECE_H
#define PIECE_H

#include "ObjectCount.h"
#include <string>
using namespace std;
class Board;

class Piece : public ObjectCount<Piece> {
 public:
  string type;
  string color;
  bool beenmoved;
  Piece(int xx, int yy, string ccolor, string typee) {
    x = xx;
    y = yy;
    color = ccolor;
    type = typee;
    beenmoved = false;
  }
  virtual void getMoves(Board* board, bool color = true) = 0;
  void move(int newx, int newy) {
    x = newx;
    y = newy;
  }
  static bool test(int i, int j, Board* board, string incolor, bool color = true);
  void moved() { beenmoved = true; }
 protected:
  int x, y;
};

#endif
