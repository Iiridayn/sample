#include "XMLIO.h"
#include <iostream>

void XMLIO::read() {
  ifstream infile(filename.c_str());
  string file;
  if(!infile){board->gui->status_bar_push_text(("Bad file").c_str()); return;}
  else {
    while((c = infile->Read()) != -1)
    file += (char)c;
  }
  board->clear();
  int front;
  while((front = file.find("<piece>")) != -1) {
    int back = file.find("/>", front);
    string type = file.substr(file.find("type=\"", front, back-front)+6, file.find("\"", file.find("type=\"", front, back-front)+6, back-front));
    string color = file.substr(file.find("color=\"", front, back-front)+7, file.find("\"", file.find("color=\"", front, back-front)+7, back-front));
    int x = atoi(file.substr(file.find("column=\"", front, back-front)+8, file.find("\"", file.find("colomn=\"", front, back-front)+8, back-front)));
    int y = atoi(file.substr(file.find("row=\"", front, back-front)+5, file.find("\"", file.find("row=\"", front, back-front)+5, back-front)));
    board->setPiece(x, y, type, color);
    file.replace(front, back-front, " ");
  } //while
  //history, prolly do same mostly
}
