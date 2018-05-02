#ifndef BOARD_H
#define BOARD_H

#include "chessgui.h"
#include "Piece.h"
#include "ObjectCount.h"
#include "chessinclude.h"
#include <iostream>
#include "Move.h"
#include <stack>
#include <set>

class Chess;

class Board : public ObjectCount<Board> {
 public:
  stack<Move*> history; //already inited
  //stack of moves for history, called history
  set<int> poss; //poss.insert();, poss.clear();
  //poss = set of possibles (tree?) ; loc stores the piece's addy in same way
  int loc;
  int wking, bking;
  bool white;
  Piece* check;
  set<int> checker;
  Board();
  Board(ChessGui* &tempgui);
  void movePiece(int x, int y, int newx, int newy, Chess* chtemp = 0);
  void getMoves(int x, int y, bool color = true);
  Piece* board[8][8];
  void setPiece(int x, int y, string type, string color);
  void removePiece(int x, int y);
  ~Board();
  ChessGui* gui;
  void clean();
  void undo();
  bool checkCheck();
  void rmbad(int x, int y);
  void colorValid();
 private:
  void init();
  void clear();
  //history
};

#endif
