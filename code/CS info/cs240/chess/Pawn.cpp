#include "Pawn.h"
#include "Board.h"

void Pawn::getMoves(Board* board, bool usecol) {
  int index = (color == "white")?y-1:y+1;
  bool temp = false;
  if(index >= 0 && index < 8) {
    if(board->board[x][index] == 0) {
      board->poss.insert((x*10)+index);
      cout << "adding: " << ((x*10)+index) << endl;
      temp = true;
    }
  }//forward 1
  if(x+1 < 8)
    if(board->board[x+1][index] != 0)
      if(board->board[x+1][index]->color != color) {
	board->poss.insert(((x+1)*10)+index);
	cout << "adding: " << (((x+1)*10)+index) << endl;
      }
  if(x-1 >= 0)
    if(board->board[x-1][index] != 0)
      if(board->board[x-1][index]->color != color) {
	board->poss.insert(((x-1)*10)+index);
	cout << "adding: " << (((x-1)*10)+index) << endl;
      }
  if(temp && ((color=="white")?(y==6):(y==1))) {
    index = index-(y-index);
    if(board->board[x][index] == 0) {
      board->poss.insert((x*10)+index);
      cout << "adding: " << ((x*10)+index) << endl;
    } // forward 2
  }
  if(usecol) {
    board->rmbad(x, y);
    board->colorValid();
  }
}

