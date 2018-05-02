#ifndef __INPUTBOX_H__
#define __INPUTBOX_H__

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

enum {IB_CANCEL=1, IB_DISMISS=1, IB_OK=0};


class InputBox : public Gtk::Dialog {
protected:

	// internal dialog widgets and structures
	Gtk::Label message;
	Gtk::Frame message_frame;
	Gtk::Label input_label;
	Gtk::Entry input_entry;

	Gtk::HBox input_hbox;
	Gtk::Button button[2];

	int event_interrupt;
	int last_button_event;


	//internal callbacks here
	void on_button_clicked(int which_button);
	gint on_destroy_event(GdkEventAny *);

	gint on_key_press_event(GdkEventKey *, int which_widget);

public:
	InputBox();
	InputBox(const char *title, const char *message, const char *entry_label);
	~InputBox();

	//signals emitted
	SigC::Signal1<void,int> button_pressed;  //all events captured and re-emitted by this one signal

	//method calls
	//hide();   // inherited from Gtk::Dialog
	//show();   // ditto
	void set_title(const char *title);
	void set_default_button(int which_button);
	void set_message(const char *message); //This sets the message only
	void set_input_label(const char *label);

	int process_events();
	int get_last_button_clicked();  //returns the last button pressed by user.
	const char *get_input_text();
};

#endif
