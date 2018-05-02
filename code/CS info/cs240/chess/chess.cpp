#if (__GNUC__ > 2)
#include <iostream>
#else
#include <iostream.h>
#endif
#include <stdio.h>
#include <string.h>
#include "chess.h"

#include "Board.h"

// The constructor for the Chess class
// Sets up a GUI object and shows the GUI on the screen
Board* board;
#ifdef CHESS
Chess::Chess() {
  gui = new ChessGui(); // a GUI object
  gui->main_window_show(); // show the GUI
  
  gui->main_window_set_title("CS240 Chess"); // sets the Title Bar
  gui->top_label_set_text("Top"); // sets the label above the chess board
  gui->bottom_label_set_text("Bottom"); // sets the label below the chess board
  
  // The connect commands connect events to functions - do not delete
  gui->button_clicked.connect(slot(this, &Chess::on_button_clicked));
  gui->mouse_button_pressed.connect(slot(this,&Chess::on_mouse_button_pressed));
  gui->mouse_button_released.connect(slot(this,&Chess::on_mouse_button_released));
  gui->cell_drag_drop.connect(slot(this,&Chess::on_drag_drop));
  gui->cell_drag_begin.connect(slot(this,&Chess::on_drag_begin));
  gui->cell_drag_enter.connect(slot(this,&Chess::on_drag_enter));
  gui->cell_drag_leave.connect(slot(this,&Chess::on_drag_leave));
  gui->main_window_close.connect(slot(this,&Chess::on_delete_event));
  
  /* // sets up the chess board with white and red squares
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
  } */ // for r
#endif

  // add your code here
  board = new Board(gui);
#ifdef CHESS
} // constructor
#endif
// Destructor deletes the GUI object
#ifdef CHESS
Chess::~Chess() {
  delete gui;
#endif
  
  // add your code here
  delete board;
#ifdef CHESS
} // destructor
#endif

// on_button_clicked is called when one of the four buttons has been clicked
// Contains example code that you are welcome to modify/delete
#ifdef CHESS
void Chess::on_button_clicked(int which_button) {
  // this test code can be deleted
  //char message[1024];
  //cout << "A button was clicked" << endl; // output to the console window
  //sprintf(message,"Button %d clicked\n",which_button);
  //gui->info_area_put_text(message); // output to the text area
  if(which_button==0) {
    //sprintf(message,"Input box contains %s\n",gui->input_box_get_text()); // gets the text from the text box
    //gui->info_area_put_text(message);
    delete board;
    board = new Board(gui);
  } // if
  else if (which_button==3) {
    //gui->info_area_put_text("Input box cleared\n");
    //gui->input_box_clear(); // clears the text box
    board->poss.clear();
    board->clean();
    board->undo();
  } // else
#endif

// add your code here

#ifdef CHESS
} // on_button_clicked
#endif
// on_mouse_button_released is called when the mouse button is released in a board square
// A good function for finding out which square was clicked on
// Contains example code that you are welcome to modify/delete
#ifdef CHESS
void Chess::on_mouse_button_released(int c, int r, int button) {
  // this test code can be deleted
  //char message[1024];
  //sprintf(message,"mbtn released %d in %d, %d\n",button,c,r);
  //gui->info_area_put_text(message);
#endif
  
  // add your code here
  if(button == 1 || button == 3) {
    if(board->poss.find((c*10)+r) != board->poss.end()) {//found
      cout << "loc: " << board->loc << endl;
      int x = board->loc/10;
      cout << "x: " << x << endl;
      int y = board->loc%10;
      cout << "y: " << y << endl;
      board->movePiece(x, y, c, r, this); //get the first two from loc
      board->clean();
      //moved, check for check and stale
      board->getMoves(c, r, false);
      if(board->white&&(board->poss.find(board->bking) != board->poss.end())) {
	cout << "Check!" << endl;
	board->check = board->board[c][r];
	board->checker = board->poss;
	message_box("Check","Black's King is in Check!", MB_OK_BOX);
      }
      else if(!board->white&&(board->poss.find(board->wking) != board->poss.end())) {
	cout << "Check!" << endl;
	board->check = board->board[c][r];
	board->checker = board->poss; 
	message_box("Check", "White's King is in Check!", MB_OK_BOX);
      }
      else board->check = 0;
      board->poss.clear();

      board->white = !board->white;	    
      if(board->white)
	gui->bottom_label_set_text("White's turn");
      else
	gui->top_label_set_text("Black's turn");
    }
    else { //(board->poss.empty()/*?*/) { //if the set is empty
      board->clean();
      board->getMoves(c, r);
    }
  }
#ifdef CHESS
} // on_mouse_button_released
#endif
// on_delete_event is called when the user closes the chess window
// Used to exit the program. Includes updated code from Nick North
#ifdef CHESS
int Chess::on_delete_event() {
#endif

// add your code here
  delete board;

#ifdef CHESS
    Gtk::Main::quit();
    return 0;
} // on_delete_event
#endif


// message_box opens a message box and returns the index of the button pushed. The return value is 1-based, so if the first button is clicked, 1 will be returned.
// Valid types of box are: MB_OK_BOX, MB_OK_CANCEL_BOX and MB_YESNO_BOX
#ifdef CHESS
int Chess::message_box(const char *title, const char *message, int box_type) {
	int button_clicked;
	MessageBox *messbox = new MessageBox();
	gui->main_window_set_active(false);
	messbox->set_title(title);
	messbox->set_message(message);
	messbox->set_type(box_type);
	messbox->show();
	button_clicked = messbox->process_events();
	messbox->hide();
	gui->main_window_set_active(true);
	delete messbox;
	return button_clicked;
} // message_box
#endif

// input_box opens a dialog that allows a user to input text. It returns the text they input.
#ifdef CHESS
const char *Chess::input_box(const char *title, const char *message, const char* label) {
	InputBox *inbox = new InputBox();
	gui->main_window_set_active(false);
	inbox->set_title(title);
	inbox->set_message(message);
	inbox->set_input_label(label);
	inbox->show();
	inbox->process_events();
	const char *input = inbox->get_input_text();
	inbox->hide();
	gui->main_window_set_active(true);
	delete inbox;
	return input;
} // input_box
#endif

// button_box opens a message box with user definable buttons (the labels of the buttons are taken from
// the array button_labels) and returns the index of the button pushed. The return value is 1-based,
// so if the first button is clicked, 1 will be returned.
#ifdef CHESS
int Chess::button_box(const char *title, const char *message, const char **button_labels, int num_buttons) {
	int button_clicked;
	ButtonBox *buttons = new ButtonBox();
	gui->main_window_set_active(false);
	buttons->set_title(title);
	buttons->set_message(message);
	buttons->set_buttons(num_buttons, button_labels);
	buttons->show();
	button_clicked = buttons->process_events();
	buttons->hide();
	gui->main_window_set_active(true);
        delete buttons;
	return button_clicked;
} // button_box
#endif

// The following functions are useful if you want to implement a drag and drop interface
#ifdef CHESS
void Chess::on_mouse_button_pressed(int c, int r, int type, int button) {
#endif
// add your code here if you are implmenting drag and drop
#ifdef CHESS
} // on_mouse_button_pressed
#endif

#ifdef CHESS
void Chess::on_drag_begin(int c, int r, int hotx, int hoty) {
#endif
// add your code here if you are implmenting drag and drop
#ifdef CHESS
} // on_drag_begin
#endif

#ifdef CHESS
void Chess::on_drag_enter(int c, int r) {
#endif
// add your code here if you are implmenting drag and drop
#ifdef CHESS
} // on_drag_enter
#endif

#ifdef CHESS
void Chess::on_drag_leave(int c, int r) {
#endif
// add your code here if you are implmenting drag and drop
#ifdef CHESS
} // on_drag_leave
#endif

#ifdef CHESS
void Chess::on_drag_drop(bool success, int sx, int sy, int tx, int ty) {
#endif
// add your code here if you are implmenting drag and drop
#ifdef CHESS
} // on_drag_drop
#endif

// run executes the run function for the GTK application which fires up the user interface
// Do not modify
#ifdef CHESS
void Chess::run(Gtk::Main &myapp, int argc, char *argv[]){
   myapp.run();
} // run
#endif

/* This is the main function of the application that you are creating
It creates the chess object and calls its run function in order to start
the user interface. Includes updated code provided by Nick North */
int main(int argc, char *argv[]) {
#ifdef CHESS
   Gtk::Main myapp(argc,argv);

   Chess *chessobj = new Chess();
   chessobj->run(myapp,argc,argv);
   delete chessobj;

   return 0;
#endif
} // main
