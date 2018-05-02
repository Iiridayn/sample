#include "buttonbox.h"

#ifdef DEBUG_BUTTON_BOX
#define DEBUG
#endif

using namespace SigC;

#define MIN_BUTTON_WIDTH 70
#define MESSAGE_FRAME_BORDER_WIDTH 10
/**
 *
 *
 */
void ButtonBox::on_button_clicked(int which_button){
	button_pressed.emit(which_button);
	last_button_event=which_button;
#ifdef DEBUG	
	g_print("button %d pressed\n",which_button);
#endif
	if(event_interrupt){
		event_interrupt=0;
		gtk_main_quit();
	}
}


/**
 *
 *
 */
gint ButtonBox::on_destroy_event(GdkEventAny *event){
	button_pressed.emit(BB_DISMISS);
	last_button_event=BB_DISMISS;
#ifdef DEBUG	
	g_print("User asked for dialog dismissal\n");
#endif
	if(event_interrupt){
		event_interrupt=0;
		gtk_main_quit();
	}
	return true; //should I return false?  can't remember!
	//we must stop the signal, since we've dealt with it
}

/**
 *
 *
 */
gint ButtonBox::on_key_press_event(GdkEventKey *kev){
	if(kev->keyval == GDK_Escape){
		on_destroy_event(NULL);
		return 1;
	}
	return 0;
}

/**
 *
 *
 */
ButtonBox::ButtonBox(){
	buttons=0;
	button=NULL;
	last_button_event=-1;
	event_interrupt=0;

	set_modal(true);
	
	message.show();
	message.set_justify(GTK_JUSTIFY_CENTER);
	message.set_line_wrap(true);

	message_frame.set_border_width(10);
	message_frame.set_shadow_type(GTK_SHADOW_NONE);
	message_frame.add(message);
	message_frame.show();
	
	buttons_vbox.pack_start(message_frame,true,true,0);
	buttons_vbox.set_spacing(5);
	buttons_vbox.show();
	
	add(buttons_vbox);
	
	set_border_width(15);
	
	
	delete_event.connect(slot(this, &ButtonBox::on_destroy_event));

}

/**
 *
 *
 */
ButtonBox::ButtonBox(const char *title,
                       const char *_message,
                       int num_buttons,
                       const char **button_text,
                       int default_button) {
	buttons=0;
	button=NULL;
	last_button_event=-1;
	event_interrupt=0;

	set_modal(true);
	
	set_message(_message);
	
	message.set_justify(GTK_JUSTIFY_CENTER);
	message.set_line_wrap(true);
	message.show();
	
	message_frame.set_border_width(10);
	message_frame.set_shadow_type(GTK_SHADOW_NONE);
	message_frame.add(message);
	message_frame.show();
	
	buttons_vbox.pack_start(message_frame,true,true,0);
	
	buttons_vbox.set_spacing(5);
	buttons_vbox.show();
	
	add(buttons_vbox);
	
	set_border_width(15);
	
	delete_event.connect(slot(this, &ButtonBox::on_destroy_event));
	
	set_buttons(num_buttons,button_text,default_button);
}


/**
 *
 *
 */
ButtonBox::~ButtonBox() {
	int x;

	if(button) {
		for(x=0;x<buttons;x++) {
			button_clicked_connection[x].disconnect();
			button_keypress_connection[x].disconnect();
			delete button[x];
		}
		delete[] button_clicked_connection;
		delete[] button_keypress_connection;
		delete[] button;
	}
	button=NULL;
	button_clicked_connection=NULL;
	button_keypress_connection=NULL;
}

/**
 *
 *
 */
void ButtonBox::set_buttons(int num_buttons, const char **button_text, int default_button) {  //This sets the box's type,only effects the button display
	int x;

	//first we need to disconnect and delete existing buttons, if any
	if(button) {
		for(x=0;x<buttons;x++) {
			button_clicked_connection[x].disconnect();
			button_keypress_connection[x].disconnect();
			buttons_vbox.remove(*(button[x]));
			delete button[x];
		}
		delete[] button_clicked_connection;
		delete[] button_keypress_connection;
		delete[] button;
	}
	button=NULL;
	button_clicked_connection=NULL;
	button_keypress_connection=NULL;
	buttons=num_buttons;
	
	//now we can create an array of button pointers for our new buttons
	//as well as connection objects so we can disconnect them later when
	//the dialog is reconfigured
	button=new (Gtk::Button *) [num_buttons];
	button_clicked_connection=new Connection[num_buttons];
	button_keypress_connection=new Connection[num_buttons];
	
	//create each button
	for(x=0;x<buttons;x++) {
		button[x]=new Gtk::Button(button_text[x]);
		buttons_vbox.pack_start(*button[x],false,false);
		button[x]->set_usize(MIN_BUTTON_WIDTH,0);
		button[x]->show();
		button_clicked_connection[x] = button[x]->clicked.connect(bind(slot(this,&ButtonBox::on_button_clicked),x+1));
		button_keypress_connection[x]= button[x]->key_press_event.connect(slot(this,&ButtonBox::on_key_press_event));
		button[x]->set_flags(GTK_CAN_FOCUS | GTK_CAN_DEFAULT);
	}
	
	//set the default button
	button[default_button-1]->grab_focus();
	button[default_button-1]->grab_default();
	
}

/**
 *
 *
 */
void ButtonBox::set_title(const char *title){
	this->Window::set_title(string(title));
}	

/**
 *
 *
 */
void ButtonBox::set_message(const char *_message){ //This sets the message only
	message.set_text(string(_message));
}

/**
 *
 *
 */
int ButtonBox::process_events(){
	//set class flag so that event handlers will end our local gtk_main loop
	//thus returning control to the caller (makes a neat and tidy block until
	//event interface, even if the caller is from a gtk event handler
	event_interrupt=1;
	
	//start up new main loop.  Our local event handlers will kill it when
	//they receive an event signal.  We're using a C gtk+ call here.
	gtk_main();
	
	//An event should have happened, so control returns
	//In this case the even can be prepresented as a simple integer, so we'll
	//return the event code (button number)
	return last_button_event;
}

/**
 *
 *
 */
int ButtonBox::get_last_button_clicked(){
	return last_button_event;
}

#ifdef DEBUG_BUTTON_BOX
int main(int argc, char *argv[]) {
	Gtk::Main myapp(argc,argv);
	static const char *mybuttons[]={"Ok","Cancel"};
	static const char *yesnobuttons[]={"Yes","No","Cancel"};

	ButtonBox myMessage("Hi there","Hello world",2,mybuttons,1);

	myMessage.show();

	myMessage.process_events();



	//myapp.run();
}
#endif
