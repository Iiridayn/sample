#ifndef CHESS_H
#define CHESS_H
#define CHESS

#include "chessgui.h"
#include "chessinclude.h"
#include "buttonbox.h"
#include "messagebox.h"
#include "inputbox.h"

class Chess : public Gtk::Widget { // Must be a GTK widget

protected:
#ifdef CHESS
   ChessGui* gui; // the object for the graphical user interface
/*
You will need to change the functions below by adding your own code to deal with the appropriate events
It would be good practice to simply have these functions call corresponding functions in a class that you
have written to control the chess game
*/
   // on_button_clicked gets called when the user clicks on one of the GUI's buttons
   // The value of button will be: 0 = New Game, 1 = Load, 2 = Save, 3 = Undo
   void on_button_clicked(int which_button);
   // on_mouse_button_released gets called when the user clicks the mouse button on a
   // square of the chess board
   void on_mouse_button_released(int c, int r, int button);
   // on_delete_event gets called when the user closes the chess window
   int on_delete_event();
/*
You don't need to implement anything for the following functions,
but they are available if you wish to create a drag and drop interface
*/
   void on_mouse_button_pressed(int c, int r, int type, int button);
   void on_drag_begin(int c, int r, int hotx, int hoty);
   void on_drag_enter(int c, int r);
   void on_drag_leave(int c, int r);
   void on_drag_drop(bool sucess, int sx, int sy, int tx, int ty);
/*
These functions allow you to create pop-up dialog boxes for interaction with the user
*/
   // message_box opens a message box and returns the index of the button pushed. The return value is 1-based, so if the first button is clicked, 1 will be returned.
   // Valid types of box are: MB_OK_BOX, MB_OK_CANCEL_BOX and MB_YESNO_BOX
   int message_box(const char *title, const char *message, int box_type=MB_OK_BOX);

   // input_box opens a dialog that allows a user to input text. It returns the text they input.
   const char *input_box(const char *title, const char *message, const char* label);

   // button_box opens a message box with user definable buttons (the labels of the buttons are taken from
   // the array button_labels) and returns the index of the button pushed. The return value is 1-based,
   // so if the first button is clicked, 1 will be returned.
#endif
public:
#ifdef CHESS
   int button_box(const char *title, const char *message, const char **button_labels, int num_buttons);
   Chess(); // constructor - creates the GUI object
   ~Chess(); // destructor - destroys the GUI object

   void run(Gtk::Main &myapp, int argc, char *argv[]); // runs the GUI
#endif
};


#endif
