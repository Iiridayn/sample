#ifndef MOVE_H
#define MOVE_H

#include "ObjectCount.h"

class Move : public ObjectCount<Move> {
 public:
  int x, y, oldx, oldy;
  string type;
  string newtype;
  string capture;
  string color;
  bool check;
  bool first;
  Move(int _x, int _y, int _oldx, int _oldy, string _type, string _color, string _newtype="") {
    x = _x; y = _y; oldx = _oldx; oldy = _oldy; type = _type; color = _color;
    if(_newtype != "") newtype = _newtype;
    capture = "";
    first = false;
  }
  void cap(string cap) {
    capture = cap;
  }
};

#endif
