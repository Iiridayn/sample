#include "messagebox.h"

#ifdef DEBUG_MESSAGE_BOX
#define DEBUG
#endif

using namespace SigC;

#define MIN_BUTTON_WIDTH 70
#define MESSAGE_FRAME_BORDER_WIDTH 10
/**
 *
 *
 */
void MessageBox::on_button_clicked(int which_button){
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
gint MessageBox::on_destroy_event(GdkEventAny *event){
	button_pressed.emit(0);
	last_button_event=0;
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
gint MessageBox::on_key_press_event(GdkEventKey *kev){
	if(kev->keyval == GDK_Escape){
		on_button_clicked(MB_DISMISS);
		return 1;
	}
	return 0;
}

/**
 *
 *
 */
MessageBox::MessageBox(){
	buttons=0;
	button=NULL;
	last_button_event=-1;
	event_interrupt=0;

	set_modal(true);
	get_action_area()->set_homogeneous(false);
	message_frame.set_border_width(10);
	message_frame.set_shadow_type(GTK_SHADOW_NONE);
	message_frame.add(message);
	message.show();
	message_frame.show();
	get_vbox()->pack_start(message_frame,true,true,0);
	
	message.set_justify(GTK_JUSTIFY_CENTER);
	message.set_line_wrap(true);
	delete_event.connect(slot(this, &MessageBox::on_destroy_event));

}

/**
 *
 *
 */
MessageBox::MessageBox(const char *title,
                       const char *_message,
                       int num_buttons,
                       const char **button_text,
                       int default_button) {
	last_button_event=-1;
	event_interrupt=0;
	buttons=0;
	button=NULL;

	set_modal(true);
	set_title(title);
	get_action_area()->set_homogeneous(false);
	message_frame.set_border_width(MESSAGE_FRAME_BORDER_WIDTH);
	message_frame.set_shadow_type(GTK_SHADOW_NONE);
	message_frame.add(message);
	message.show();
	message_frame.show();
	get_vbox()->pack_start(message_frame,true,true,0);
	
	message.set_justify(GTK_JUSTIFY_CENTER);
	message.set_line_wrap(true);
	set_message(_message);
	set_buttons(num_buttons,button_text,default_button);
	delete_event.connect(slot(this, &MessageBox::on_destroy_event));
}

/**
 *
 *
 */
MessageBox::MessageBox(const char *title,
                       const char *_message,
                       int type,
                       int default_button) {
	last_button_event=-1;
	event_interrupt=0;
	buttons=0;
	button=NULL;

	set_modal(true);
	get_action_area()->set_homogeneous(false);
	message_frame.set_border_width(MESSAGE_FRAME_BORDER_WIDTH);
	message_frame.set_shadow_type(GTK_SHADOW_NONE);
	message_frame.add(message);
	message.show();
	message_frame.show();
	get_vbox()->pack_start(message_frame,true,true,0);
	
	message.set_justify(GTK_JUSTIFY_CENTER);
	message.set_line_wrap(true);
	set_message(_message);
	set_title(title);
	set_type(type,default_button);
	
	delete_event.connect(slot(this, &MessageBox::on_destroy_event));
}

/**
 *
 *
 */
MessageBox::~MessageBox() {
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
void MessageBox::set_buttons(int num_buttons, const char **button_text, int default_button) {  //This sets the box's type,only effects the button display
	int x;

	//first we need to disconnect and delete existing buttons, if any
	if(button) {
		for(x=0;x<buttons;x++) {
			button_clicked_connection[x].disconnect();
			button_keypress_connection[x].disconnect();
			get_action_area()->remove(*(button[x]));
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
	
	//just in case
	get_action_area()->show();
	
	
	//create each button
	for(x=0;x<buttons;x++) {
		button[x]=new Gtk::Button(button_text[x]);
		get_action_area()->pack_start(*button[x],false,false);
		button[x]->set_usize(MIN_BUTTON_WIDTH,0);
		button[x]->show();
		button_clicked_connection[x] = button[x]->clicked.connect(bind(slot(this,&MessageBox::on_button_clicked),x+1));
		button_keypress_connection[x]= button[x]->key_press_event.connect(slot(this,&MessageBox::on_key_press_event));
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
void MessageBox::set_title(const char *title){
	this->Window::set_title(string(title));
}	

/**
 *
 *
 */
void MessageBox::set_type(int type,int default_button){
	switch(type){
	case MB_OK_BOX:
		set_buttons(1,mb_ok_buttons,default_button);
		break;
	case MB_OK_CANCEL_BOX:
		set_buttons(2,mb_ok_cancel_buttons,default_button);
		break;
	case MB_YESNO_BOX:
		set_buttons(2,mb_yesno_buttons,default_button);
		break;
	}
}

/**
 *
 *
 */
void MessageBox::set_message(const char *_message){ //This sets the message only
	message.set_text(string(_message));
}

/**
 *
 *
 */
int MessageBox::process_events(){
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
int MessageBox::get_last_button_clicked(){
	return last_button_event;
}

#ifdef DEBUG_MESSAGE_BOX
int main(int argc, char *argv[]) {
	Gtk::Main myapp(argc,argv);
	static const char *mybuttons[]={"Ok","Cancel"};
	static const char *yesnobuttons[]={"Yes","No","Cancel"};

	MessageBox myMessage("Hi there","Hello world",2,mybuttons,1);

	myMessage.show();

	myMessage.process_events();
	if(myMessage.get_last_button_clicked()==2){
		myMessage.set_title("Are you sure?");
		myMessage.set_message("Are you sure?  This is a really big decision in your life.  I wouldn't want you to make the wrong one.");
  	}else{
		myMessage.set_title("Really?");
		myMessage.set_message("Do you mean okay, or really okay?");
	}
	myMessage.set_buttons(3,yesnobuttons,1);
	myMessage.process_events();
	
	myMessage.set_type(MB_OK_BOX);
	myMessage.set_message("This is the Okay box");
	myMessage.set_title("OkBox");
	myMessage.process_events();
	
	myMessage.set_type(MB_OK_CANCEL_BOX);
	myMessage.set_message("This is the Okay/Cancel box");
	myMessage.set_title("OkCanceBox");
	myMessage.process_events();

	myMessage.set_type(MB_YESNO_BOX);
	myMessage.set_message("This is the Yes/No box");
	myMessage.set_title("YesNoBox");
	myMessage.process_events();



	//myapp.run();
}
#endif
