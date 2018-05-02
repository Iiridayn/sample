#ifndef __MESSAGEBOX_H__
#define __MESSAGEBOX_H__

#include <sigc++/signal_system.h>
#include <gdk/gdkkeysyms.h>

using namespace std;
//using namespace SigC;



#include <gtk--/base.h>
#include <gtk--/main.h>
#include <gtk--/style.h>
#include <gtk--/window.h>
#include <gtk--/dialog.h>
#include <gtk--/button.h>
#include <gtk--/box.h>
#include <gtk--/label.h>
#include <gtk--/entry.h>
#include <gtk--/statusbar.h>
#include <gtk--/entry.h>
#include <gtk--/frame.h>
#include <gtk--/table.h>
#include <gtk--/text.h>
#include <gtk--/drawingarea.h>
#include <gtk--/separator.h>
#include <gtk--/scrolledwindow.h>


/***
 * Note that DISMISS is a 0!!! Cancel is a 2!!!  This way
 * dismissals are always consistant no matter how many buttons
 * you stick in there!  If dismiss and cancel do the same for
 * your program, be sure to check for both!!!!
 */
enum {MB_DISMISS=0, MB_YES=1, MB_OK=1, MB_NO=2, MB_CANCEL=2};
enum {MB_OK_BOX=0, MB_OK_CANCEL_BOX=1, MB_YESNO_BOX=2, MB_CUSTOM_BOX=3};

static const char *mb_ok_buttons[]={"Ok"};
static const char *mb_ok_cancel_buttons[]={"Ok","Cancel"};
static const char *mb_yesno_buttons[]={"Yes","No"};

class MessageBox : public Gtk::Dialog {
protected:
	
	// internal dialog widgets and structures
	Gtk::Label message;
	Gtk::HBox buttons_hbox;
	Gtk::Frame message_frame;
	Gtk::Button **button;
	SigC::Connection *button_clicked_connection;
	SigC::Connection *button_keypress_connection;
	int event_interrupt;
	int last_button_event;
	int buttons;


	//internal callbacks here
	void on_button_clicked(int which_button);
	gint on_destroy_event(GdkEventAny *);
	gint on_key_press_event(GdkEventKey *);
public:
	MessageBox();
	MessageBox(const char *title, const char *message, int mb_type, int default_button);
	MessageBox(const char *title, const char *message, int num_buttons, const char **button_text, int default_button=1);
	~MessageBox();

	//signals emitted
	SigC::Signal1<void,int> button_pressed;  //all events captured and re-emitted by this one signal

	//method calls
	//hide();   // inherited from Gtk::Dialog
	//show();   // ditto
	void set_title(const char *title);
	void set_type(int type, int default_button=1);
	void set_buttons(int num_buttons, const char **button_text, int default_button=1);  //This sets the box's type,only effects the button display
	void set_default_button(int which_button);
	void set_message(const char *message); //This sets the message only

	int process_events();
	int get_last_button_clicked();  //returns the last button pressed by user.
	
};

#endif
