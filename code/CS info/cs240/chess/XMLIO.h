#ifndef XMLIO_H
#define XMLIO_H

#include "ObjectCount.h"

class XMLIO : public ObjectCount<XMLIO> {
 public:
  XMLIO(string file, Board* thing, bool read = true) {
    filename = file;
    board = thing;
    if(read) read();
    else write();
  }
 private:
  string filename;
  Board* board;
  void read();
  void write();
}

#endif
