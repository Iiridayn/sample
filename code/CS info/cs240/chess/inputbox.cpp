#include "inputbox.h"

#ifdef DEBUG_INPUT_BOX
#define DEBUG
#endif


#define MESSAGE_FRAME_BORDER_WIDTH 10
#define INPUT_BORDER_WIDTH 5
#define MIN_BUTTON_WIDTH 70
#define BUTTON_EVENT_FLAG 1
#define ENTRY_EVENT_FLAG 0

void InputBox::on_button_clicked(int which_button){
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
gint InputBox::on_destroy_event(GdkEventAny *event){
	button_pressed.emit(IB_DISMISS);
	last_button_event=IB_DISMISS;
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

gint InputBox::on_key_press_event(GdkEventKey *kev, int which_widget){
	if(kev->keyval == GDK_Return && which_widget==ENTRY_EVENT_FLAG){
		if(button[0].has_default())
			on_button_clicked(0);
		else
			on_button_clicked(1);
			
		return 1;
	}else if(kev->keyval == GDK_Escape){
		on_destroy_event(NULL);
		return 1;
	}
	return 0;
}

InputBox::InputBox(){
	last_button_event=-1;
	event_interrupt=0;
	
	message.set_text(string(""));
	message.show();
	
	message_frame.set_shadow_type(GTK_SHADOW_NONE);
	message_frame.set_border_width(MESSAGE_FRAME_BORDER_WIDTH);
	message_frame.add(message);
	message_frame.show();
	
	get_vbox()->pack_start(message_frame,false,false,0);
	
	input_label.show();
	
	input_entry.show();
	input_entry.key_press_event.connect(bind(slot(this,&InputBox::on_key_press_event),ENTRY_EVENT_FLAG));
	
	input_hbox.show();
	input_hbox.set_border_width(INPUT_BORDER_WIDTH);
	input_hbox.set_spacing(5);
	input_hbox.pack_start(input_label,false,false,0);
	input_hbox.pack_start(input_entry,true,true,0);
	
	get_vbox()->pack_start(input_hbox,true,true,0);

	button[0].add_label(string("Ok"),0.5);	
	button[0].set_usize(MIN_BUTTON_WIDTH,0);
	button[0].set_flags(GTK_CAN_FOCUS | GTK_CAN_DEFAULT);
	button[0].clicked.connect(bind(slot(this,&InputBox::on_button_clicked),0));
	button[0].key_press_event.connect(bind(slot(this,&InputBox::on_key_press_event),BUTTON_EVENT_FLAG));
	button[0].show();
	
	button[1].add_label(string("Cancel"),0.5);	
	button[1].set_usize(MIN_BUTTON_WIDTH,0);
	button[1].set_flags(GTK_CAN_FOCUS | GTK_CAN_DEFAULT);
	button[1].clicked.connect(bind(slot(this,&InputBox::on_button_clicked),1));
	button[1].key_press_event.connect(bind(slot(this,&InputBox::on_key_press_event),BUTTON_EVENT_FLAG));
	button[1].show();
	
	get_action_area()->set_homogeneous(false);
	get_action_area()->pack_start(button[0],false,false,0);
	get_action_area()->pack_start(button[1],false,false,0);
	get_action_area()->show();
	button[0].grab_default(); //but not focus!
	input_entry.grab_focus();
}

InputBox::InputBox(const char *title, const char *_message, const char *_input_label){
	last_button_event=-1;
	event_interrupt=0;

	this->Window::set_title(string(title));
	message.set_text(string(_message));
	message.show();
	
	message_frame.set_shadow_type(GTK_SHADOW_NONE);
	message_frame.set_border_width(MESSAGE_FRAME_BORDER_WIDTH);
	message_frame.add(message);
	message_frame.show();
	
	get_vbox()->pack_start(message_frame,false,false,0);
	
	input_label.set_text(string(_input_label));
	input_label.show();
	
	input_entry.show();
	input_entry.key_press_event.connect(bind(slot(this,&InputBox::on_key_press_event),ENTRY_EVENT_FLAG));
	
	input_hbox.show();
	input_hbox.set_border_width(INPUT_BORDER_WIDTH);
	input_hbox.set_spacing(5);
	input_hbox.pack_start(input_label,false,false,0);
	input_hbox.pack_start(input_entry,true,true,0);
	
	get_vbox()->pack_start(input_hbox,true,true,0);
	
	button[0].add_label(string("Ok"),0.5);	
	button[0].set_usize(MIN_BUTTON_WIDTH,0);
	button[0].set_flags(GTK_CAN_FOCUS | GTK_CAN_DEFAULT);
	button[0].clicked.connect(bind(slot(this,&InputBox::on_button_clicked),0));
	button[0].key_press_event.connect(bind(slot(this,&InputBox::on_key_press_event),BUTTON_EVENT_FLAG));
	button[0].show();
	
	button[1].add_label(string("Cancel"),0.5);	
	button[1].set_usize(MIN_BUTTON_WIDTH,0);
	button[1].set_flags(GTK_CAN_FOCUS | GTK_CAN_DEFAULT);
	button[1].clicked.connect(bind(slot(this,&InputBox::on_button_clicked),1));
	button[1].key_press_event.connect(bind(slot(this,&InputBox::on_key_press_event),BUTTON_EVENT_FLAG));
	button[1].show();
	
	get_action_area()->pack_start(button[0],false,false,0);
	get_action_area()->pack_start(button[1],false,false,0);
	get_action_area()->show();
	button[0].grab_default(); //but not focus!
	input_entry.grab_focus();

}

InputBox::~InputBox(){

}

void InputBox::set_title(const char *title){
	this->Window::set_title(string(title));
}

void InputBox::set_default_button(int which_button){
	button[which_button].grab_default();
}

void InputBox::set_message(const char *_message){ //This sets the message only
	message.set_text(string(_message));
}
void InputBox::set_input_label(const char *label){
	input_label.set_text(string(label));
}

int InputBox::process_events(){
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

int InputBox::get_last_button_clicked(){  //returns the last button pressed by user.
	return last_button_event;
}

const char *InputBox::get_input_text(){
	return input_entry.get_text().c_str();
}


#ifdef DEBUG_INPUT_BOX
int main(int argc, char *argv[]){

	Gtk::Main myApp(argc,argv);

	InputBox test;

	test.show();
	test.set_title("This is a test");
	test.set_message("Please enter your name for me:");
	test.set_input_label("Full name:");
	test.show();
	test.process_events();


}

#endif
