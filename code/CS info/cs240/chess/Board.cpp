#include "Board.h"
#include "King.h"
#include "Queen.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "Pawn.h"
#include "chessgui.h"
#include "Piece.h"
#include <iostream>
#include "chess.h"

void Board::init() {
  clean();
  white = true;
  check = 0;
  bking = 40;
  wking = 47;
  int x = 0;
  int y = 0;
  for(y = 0; y < 8; y++)
  for(x = 0; x < 8; x++) {
    if(y == 1)
      setPiece(x, y, "pawn", "black");
    else if(y == 6)
      setPiece(x, y, "pawn", "white");
    else if(y == 0 || y == 7)
      switch(x) {
      case 0:
      case 7:
	setPiece(x, y, "rook", (y==0)?"black":"white");
	break;
      case 1:
      case 6:
	setPiece(x, y, "knight", (y==0)?"black":"white");
	break;
      case 2:
      case 5:
	setPiece(x, y, "bishop", (y==0)?"black":"white");
	break;
      case 3:
	setPiece(x, y, "queen", (y==0)?"black":"white");
	break;
      case 4:
	setPiece(x, y, "king", (y==0)?"black":"white");
	break;
      }
    else
      board[x][y] = 0;
  }
}

void Board::setPiece(int x, int y, string type, string color) {
  if(type == "king") {
    (color == "black")?gui->board_cell_set_pixmap(x, y, BLACK_KING):gui->board_cell_set_pixmap(x, y, WHITE_KING);
    board[x][y] = new King(x, y, color);
  }
  else if(type == "queen") {
    (color == "black")?gui->board_cell_set_pixmap(x, y, BLACK_QUEEN):gui->board_cell_set_pixmap(x, y, WHITE_QUEEN);
    board[x][y] = new Queen(x, y, color);
  }
  else if(type == "rook") {
    (color == "black")?gui->board_cell_set_pixmap(x, y, BLACK_ROOK):gui->board_cell_set_pixmap(x, y, WHITE_ROOK);
    board[x][y] = new Rook(x, y, color);
  }
  else if(type == "knight") {
    (color == "black")?gui->board_cell_set_pixmap(x, y, BLACK_KNIGHT):gui->board_cell_set_pixmap(x, y, WHITE_KNIGHT);
    board[x][y] = new Knight(x, y, color);
  }
  else if(type == "bishop") {
    (color == "black")?gui->board_cell_set_pixmap(x, y, BLACK_BISHOP):gui->board_cell_set_pixmap(x, y, WHITE_BISHOP);
    board[x][y] = new Bishop(x, y, color);
  }
  else if(type == "pawn") {
    (color == "black")?gui->board_cell_set_pixmap(x, y, BLACK_PAWN):gui->board_cell_set_pixmap(x, y, WHITE_PAWN);
    board[x][y] = new Pawn(x, y, color);
  }
  else {
    return;
  }
}

void Board::getMoves(int x, int y, bool color) {
  poss.clear();
  cout << "getMoves: " << x << ", " << y << endl;
  loc = (x*10)+y;
  if(board[x][y] != 0) {
    if(board[x][y]->color != ((white)?"white":"black")) return;
    board[x][y]->getMoves(this, color);
  }
  //cout << "good" << endl;
  return;
}

void Board::clear() {
  for(int y = 0; y < 8; y++)
    for(int x = 0; x < 8; x++)
      if(board[x][y] != 0) {
	gui->board_cell_clear_pixmap(x, y);
	delete board[x][y];
      }
  clean();
}

Board::Board() { init(); }
Board::Board(ChessGui* &tempgui) {
  gui = tempgui;
  init();
}
Board::~Board() { clear(); }

void Board::clean() {
  gui->top_label_set_text("");
  gui->bottom_label_set_text("");
  gui->info_area_clear();
  for(int r=0;r<8;r++) {
    for(int c=0;c<8;c++) {
      if(r%2 == 0) {
	if (c%2 == 0) {
	  gui->board_cell_color(c,r,WHITE_SQUARE);
	} // if
	else {
	  gui->board_cell_color(c,r,RED_SQUARE);
	} // else
      } // if
      else {
	if (c%2 == 1) {
	  gui->board_cell_color(c,r,WHITE_SQUARE);
	} // if
	else {
	  gui->board_cell_color(c,r,RED_SQUARE);
	} // else
      } // else
    } // for c
  } // for r
}

void Board::undo() {
  if(history.empty()) return;
  Move* move = history.top();
  history.pop();//pop top move, call it move
  //if(move == 0) return;
  removePiece(move->x, move->y);
  //cout << "type: " << move->type << endl;
  //cout << "newtype: " << move->newtype << endl;
  setPiece(move->oldx, move->oldy, move->type, move->color);
  white = (move->color == "white");
  if(!move->first) board[move->oldx][move->oldy]->moved();
  if(move->capture != "")
    setPiece(move->x, move->y, move->capture, (move->color == "black")?"white":"black");
  delete move;
}

void Board::removePiece(int x, int y) {
  delete board[x][y];
  board[x][y] = 0;
  gui->board_cell_clear_pixmap(x, y);
}

void Board::movePiece(int x, int y, int newx, int newy, Chess* chtemp) {
  //cout << "Move: " << x << ", " << y << ", " << newx << ", " << newy << endl;
  string cap = "";
  if(board[newx][newy] != 0) {
    cap = board[newx][newy]->type;
    removePiece(newx, newy);
  }
  string type = board[x][y]->type;
  string color = board[x][y]->color;
  string oldt = type;
  bool first = board[x][y]->beenmoved;
  removePiece(x, y);
  if(type == "pawn") {
    if(chtemp!=0 && ((newy == 0 && white) || (newy == 7 && !white))) {
      const char* labels[] = {"Queen", "Rook", "Knight", "Bishop"};
      switch(chtemp->button_box("Pawn Promotion", "What Piece would you like?", labels, 4)) {
      case 1:
	type = "queen";
	break;
      case 2:
	type = "rook";
	break;
      case 3:
	type = "knight";
	break;
      case 4:
	type = "bishop";
	break;      
      }//do pawn promotion
    }
  }
  else type = "";
  if(oldt == "king") {
    ((white)?wking = ((newx*10)+newy):bking = ((newx*10)+newy));
    cout << "King to: " << ((white)?wking:bking) << endl;
  }
  setPiece(newx, newy, (type == "")?oldt:type, color);
  board[newx][newy]->moved();
  //cout << "type: " << type << endl;
  //cout << "oldt: " << oldt << endl;
  Move* temp = new Move(newx, newy, x, y, oldt, color, type);
  temp->first = first;
  if(cap != "")
    temp->cap(cap);
  history.push(temp);//push move
}

bool Board::checkCheck() {
  check->getMoves(this, false);
  checker = poss;
  if(!white&&(checker.find(bking) != checker.end())) return true;
  else if(white&&(checker.find(wking) != checker.end())) return true;
  return false;
}

void Board::rmbad(int x, int y) {
  if(check==0)return;
  set<int>::iterator it;
  set<int> tempmoves = poss;
  it = tempmoves.begin();
  while(it != tempmoves.end()) {
    //cout << (*it) << endl;
    int i = (*it)/10;
    int j = (*it)%10;
    //move for each
    cout << "testing: " << x << ", " << y << ", " << i << ", " << j << endl;
    movePiece(x, y, i, j);
    if(checkCheck()) {
      tempmoves.erase(it);
      it=tempmoves.begin();
      cout << "failed" << endl;
    }
    else cout << "isgood" << endl;
    undo();
    if(it != tempmoves.begin())
      it++;
    //if(it++ == tempmoves.end()) break;
  }
  poss = tempmoves;
}

void Board::colorValid() {
  set<int>::iterator it;
  for(it = poss.begin(); it != poss.end(); it++) {
    //cout << (*it) << endl;
    int i = (*it)/10;
    int j = (*it)%10;
    gui->board_cell_color(i, j, YELLOW_SQUARE);
  }
}
