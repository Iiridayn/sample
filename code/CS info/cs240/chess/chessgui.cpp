#include "chessgui.h"

using namespace SigC;

#ifndef CELL_SIZE
#define CELL_SIZE 64
#endif

#ifndef RIGHT_PANE_SIZE
#define RIGHT_PANE_SIZE 350
#endif

#ifndef SCALING_METHOD
#define SCALING_METHOD GDK_INTERP_BILINEAR
#endif

ChessGui::ChessGui(){
	gdk_rgb_init();
	set_title("BYU - Chess - CS 240");

   main_vbox.show();
   add(main_vbox);

   hbox.show();
   main_vbox.pack_start(hbox,true,true,0);
   hbox.set_border_width(5);

   left_pane.show();
   hbox.pack_start(left_pane,true,true,0);

   label1.show();
   label1.set_text(string("Testing"));
   label1.set_justify(GTK_JUSTIFY_LEFT);
   left_pane.pack_start(label1,false,false,0);


   board_frame=new Gtk::AspectFrame(0.5,0.5,1,false);
   board_frame->show();
   board_frame->set_border_width(5);
   board_frame->set_shadow_type(GTK_SHADOW_OUT);
   left_pane.pack_start(*board_frame,true,true,0);

   board_table.resize(MAX_BOARD_ROWS,MAX_BOARD_COLUMNS);
   board_table.set_homogeneous(true);
   board_table.set_spacings(0);
   board_table.show();
   board_frame->add(board_table);

   //configure drawing areas here
   for(int c=0; c<MAX_BOARD_COLUMNS; c++ ){
      for( int r=0; r<MAX_BOARD_ROWS; r++){
//			cell_aspect_frame[c][r] = new Gtk::AspectFrame(0.5,0.5,1,false);
//			cell_aspect_frame[c][r]->set_shadow_type(GTK_SHADOW_NONE);
//			cell_aspect_frame[c][r]->show();
//			cell_aspect_frame[c][r]->set_border_width(0);	
         cell[c][r].size(CELL_SIZE,CELL_SIZE);
         cell[c][r].show();
//         cell[c][r].set_usize(64,64);
         cell[c][r].set_events (GDK_EXPOSURE_MASK
                | GDK_LEAVE_NOTIFY_MASK
                | GDK_BUTTON_PRESS_MASK
                | GDK_BUTTON_RELEASE_MASK
                | GDK_POINTER_MOTION_MASK
                | GDK_ENTER_NOTIFY_MASK
                | GDK_LEAVE_NOTIFY_MASK);
//                | GDK_POINTER_MOTION_HINT_MASK);

         cell[c][r].configure_event.connect(bind(slot(this,&ChessGui::on_cell_configure),c,r));
         cell[c][r].expose_event.connect(bind(slot(this,&ChessGui::on_cell_expose),c,r));
         cell[c][r].motion_notify_event.connect(bind(slot(this,&ChessGui::on_mouse_motion),c,r));
         cell[c][r].button_release_event.connect(bind(slot(this,&ChessGui::on_mouse_button_released),c,r));
         cell[c][r].button_press_event.connect(bind(slot(this,&ChessGui::on_mouse_button_pressed),c,r));
                                                                                 cell[c][r].drag_begin.connect(bind(slot(this,&ChessGui::on_cell_drag_begin),c,r));
         cell[c][r].drag_motion.connect(bind(slot(this,&ChessGui::on_cell_drag_motion),c,r));
         cell[c][r].drag_end.connect(bind(slot(this,&ChessGui::on_cell_drag_end),c,r));
         cell[c][r].drag_leave.connect(bind(slot(this,&ChessGui::on_cell_drag_leave),c,r));
         cell[c][r].drag_drop.connect(bind(slot(this,&ChessGui::on_cell_drag_drop),c,r));

#ifndef USE_GDK_PIXBUF
         cell_color[c][r]=0x00000000;
#endif
//         cell_aspect_frame[c][r]->add(cell[c][r]);
         board_table.attach(cell[c][r],c,c+1,r,r+1);

         for(int i=0;i<MAX_LAYERS;i++){
            cell_user_visible[c][r][i]=false;
#ifdef USE_GDK_PIXBUF
				cell_user_visible[c][r][i]=0; //merge this with the alpha value
				cell_user_pixmap[c][r][i]=NULL;
				cell_color_alpha[c][r][i]=0;
#endif
         }
#ifdef USE_GDK_PIXBUF
			cell_backing_store[c][r]=NULL;
#endif			

      }
   }

   chess_gui_target_entry[0].target=CHESS_GUI_DND_TARGET;
   chess_gui_target_entry[0].flags=GTK_TARGET_SAME_APP;
   chess_gui_target_entry[0].info=CHESS_GUI_DND_INFO;

   label2.show();
   label2.set_text(string("Testing"));
   label2.set_justify(GTK_JUSTIFY_LEFT);
   left_pane.pack_start(label2,false,false,0);

   separator.show();
   left_pane.pack_start(separator,false,false,0);

   filename_entry_hbox.show();
   filename_entry_hbox.set_border_width(6);
   filename_entry_hbox.set_spacing(5);
   left_pane.pack_start(filename_entry_hbox,false,false,5);

   filename_entry_label.set_text(string(DEFAULT_FILENAME_ENTRY_LABEL));
   filename_entry_label.show();
   filename_entry_hbox.pack_start(filename_entry_label,false,false,0);

   filename_entry_box.set_text(string(""));
   filename_entry_box.show();
   filename_entry_hbox.pack_start(filename_entry_box,true,true,0);

   right_pane.show();
   right_pane.set_usize(RIGHT_PANE_SIZE,-2); //??-2??
   hbox.pack_start(right_pane,false,false,0);

   button_frame.show();
   button_frame.set_shadow_type(GTK_SHADOW_NONE);
   right_pane.pack_start(button_frame,false,false,0);

   button_vbox.show();
   button_vbox.set_spacing(5);
   button_vbox.set_border_width(20);
   button_frame.add(button_vbox);

   for(int x=0;x<MAX_BUTTONS;x++){
      button[x].show();
      button_vbox.pack_start(button[x],false,false,0);
      button[x].clicked.connect(bind(slot(this,&ChessGui::on_button_clicked),x));
   }
/*   button[0].add_label(string("Load Game"),0.5);
   button[1].add_label(string("Save Game"),0.5);
   button[2].add_label(string("Undo Move"),0.5);
   button[3].add_label(string("Quit"),0.5);
*/
   button[0].add_label(string("New Game"),0.5);
   button[1].add_label(string("Load Game"),0.5);
   button[2].add_label(string("Save Game"),0.5);
   button[3].add_label(string("Undo Move"),0.5);
   info_frame.set_label(string(" History and Messages "));
   info_frame.show();
   info_frame.set_border_width(5);
   right_pane.pack_start(info_frame,true,true,0);

   info_scrolled_window.show();
   info_scrolled_window.set_border_width(5);
   info_scrolled_window.set_policy(GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
   info_frame.add(info_scrolled_window);

   info_text.show();
   info_scrolled_window.add(info_text);

   status_bar.show();

   delete_event.connect(slot(this,&ChessGui::on_delete_event));

   main_vbox.pack_start(status_bar,false,false,0);



}

ChessGui::~ChessGui(){

	delete board_frame;
   //cleanup pixmap backing stores here

}

void ChessGui::on_button_clicked(int which_one){
//   cout << "You pressed button " << which_one << endl;
   //emit signal
   button_clicked.emit(which_one);
}

gint ChessGui::on_mouse_motion(GdkEventMotion *motion,int c, int r){
//   cout << "motion in " << c << "," << r;
//   cout.flush();

   //emit signal

   return true;
}

gint ChessGui::on_mouse_button_released(GdkEventButton *button, int c, int r){

   //simply emit signal, that's all.
   mouse_button_released.emit(c,r,button->button);
   return true;
}

gint ChessGui::on_mouse_button_pressed(GdkEventButton *button, int c, int r){
   int type;

   //simply emit signal, that's all.


   hot_x=(int)button->x;
   hot_y=(int)button->y;
   mouse_button_pressed.emit(c,r,type,button->button);

   return false;
}


gint ChessGui::on_cell_expose(GdkEventExpose *event, int c, int r){
   Gdk_GC my_gc;

//   cout << "expose " << c << "," << r << endl;

//   on_cell_configure(NULL,c,r);
#ifdef USE_GDK_PIXBUF
   my_gc=cell[c][r].get_style()->get_fg_gc(cell[c][r].get_state());
	gdk_pixbuf_render_to_drawable(cell_backing_store[c][r],
	                                    (GdkDrawable *)cell[c][r].get_window().gdkobj(),
	                                    my_gc,
	                                    0,0,0,0,
	                                    gdk_pixbuf_get_width(cell_backing_store[c][r]),
	                                    gdk_pixbuf_get_height(cell_backing_store[c][r]),
	                                    GDK_RGB_DITHER_NORMAL,
	                                    0,0);

#else
   my_gc=cell[c][r].get_style()->get_fg_gc(cell[c][r].get_state());
   cell[c][r].get_window().draw_pixmap(my_gc,
                                       cell_backing_store[c][r],
                                       (gint)event->area.x, (gint)event->area.y,
                                       (gint)event->area.x, (gint)event->area.y,
                                       (gint)event->area.width, (gint)event->area.height);
#endif

   return false;  //continue signal propagation
}

gint ChessGui::on_cell_configure(GdkEventConfigure *, int c, int r){
   Gdk_GC my_gc;
   Gdk_Color my_color;
   int i,x,y;
#ifdef USE_GDK_PIXBUF
	GdkPixbuf *scaled;
	GdkPixmap *tpm=NULL;
	GdkBitmap *tbm=NULL;
	guchar *colordata=NULL;
	//void (*testnull)(guchar *, void *);

	//testnull=NULL;
   Gdk_Pixmap tempcolor;

	if(cell_backing_store[c][r]) gdk_pixbuf_unref(cell_backing_store[c][r]);



	//memory leak.  Need a static method to free it!!
	//use a static GdkPixBufdestroyNotify function!!.

	colordata=new guchar[cell[c][r].width()*cell[c][r].height()*4];
	for(x=0;x<cell[c][r].width()*cell[c][r].height()*4;x+=4){
		colordata[x]  =0; //red
		colordata[x+1]=0; //green
		colordata[x+2]=0; //blue
		colordata[x+3]=255; //alpha
	}
//*
	cell_backing_store[c][r]=gdk_pixbuf_new_from_data(colordata,
					 GDK_COLORSPACE_RGB,
					 true,
					 8,
					 cell[c][r].width(),
					 cell[c][r].height(),
					 cell[c][r].width()*4,
					 NULL,
                                         NULL);
/*/
	cell_backing_store[c][r]=gdk_pixbuf_new(GDK_COLORSPACE_RGB,
					 true,
					 8,
					 cell[c][r].width(),
					 cell[c][r].height());
*/

#else
   cell_backing_store[c][r].create(cell[c][r].get_window(),cell[c][r].width(),cell[c][r].height());


   my_gc=Gdk_GC(cell_backing_store[c][r]);

   my_color.set_red((cell_color[c][r] & 0x00ff0000) >> 8 | 0x000000ff);
   my_color.set_green(cell_color[c][r] & 0x0000ff00 | 0x000000ff);
   my_color.set_blue((cell_color[c][r] & 0x000000ff) << 8 | 0x000000ff);

   cell[c][r].get_colormap().alloc(my_color);
   my_gc.set_foreground(my_color);

   cell_backing_store[c][r].draw_rectangle(my_gc,true,0,0,cell[c][r].width(),cell[c][r].height());
#endif

   for(i=MAX_LAYERS-1;i>=0;i--){
      if(cell_user_pixmap[c][r][i] && cell_user_visible[c][r][i]){
			//g_print("we have a pixmap\n");
#ifdef USE_GDK_PIXBUF
			//we'll scale the thing and then draw it.
			scaled=gdk_pixbuf_scale_simple(cell_user_pixmap[c][r][i],
			                        gdk_pixbuf_get_width(cell_backing_store[c][r]),
			                        gdk_pixbuf_get_height(cell_backing_store[c][r]),
			                        SCALING_METHOD);

			gdk_pixbuf_composite(scaled,
			                     cell_backing_store[c][r],
			                     0,0,
			                     gdk_pixbuf_get_width(scaled),
			                     gdk_pixbuf_get_height(scaled),
			                     0,0,1,1,
			                     SCALING_METHOD,
			                     cell_user_pixmap_alpha[c][r][i]);

			gdk_pixbuf_unref(scaled);
#else
         x=(cell[c][r].width()-cell_user_pixmap[c][r][i].width())/2;
         y=(cell[c][r].height()-cell_user_pixmap[c][r][i].height())/2;
//         cout << "Trying..." << i<< " " << x << " " << y <<endl;
         my_gc=cell[c][r].get_style()->get_fg_gc(cell[c][r].get_state());
         my_gc.set_clip_mask(cell_user_bitmap[c][r][i]);
         my_gc.set_clip_origin(x,y);

         cell_backing_store[c][r].draw_pixmap(my_gc,
                                              cell_user_pixmap[c][r][i],
                                              0,0,x,y,
                                              cell_user_pixmap[c][r][i].width(),
                                              cell_user_pixmap[c][r][i].height());
         my_gc.set_clip_mask();
#endif
      }
#ifdef USE_GDK_PIXBUF
		if(cell_color_alpha[c][r][i]){
			//g_print("we have a color\n");
			colordata=new guchar[cell[c][r].width()*cell[c][r].height()*4];
			for(x=0;x<cell[c][r].width()*cell[c][r].height()*4;x+=4){
				colordata[x]  = (cell_color[c][r][i] & 0x00ff0000) >> 16; //red
				colordata[x+1]= (cell_color[c][r][i] & 0x0000ff00) >> 8;  //green
				colordata[x+2]= (cell_color[c][r][i] & 0x000000ff);       //blue
				colordata[x+3]=cell_color_alpha[c][r][i]; //alpha
			}
//*
			scaled=gdk_pixbuf_new_from_data(colordata,
					 GDK_COLORSPACE_RGB,
					 true,
					 8,
					 cell[c][r].width(),
					 cell[c][r].height(),
					 cell[c][r].width()*4,
					 NULL,
                                         NULL);

			gdk_pixbuf_composite(scaled,
			                     cell_backing_store[c][r],
			                     0,0,
			                     gdk_pixbuf_get_width(scaled),
			                     gdk_pixbuf_get_height(scaled),
			                     0,0,1,1,
			                     SCALING_METHOD,
			                     255);//cell_color_alpha[c][r][i]);
			//g_print("Paintint at %d,%d,%d to color %06x, alpha %d\n",c,r,i,cell_color[c][r][i],cell_color_alpha[c][r][i]);
			gdk_pixbuf_unref(scaled);
			delete colordata;
		}
#endif


   }

   if(c==0 && r==0){
      cell_resized.emit(cell[c][r].width(),cell[c][r].height());
   }

   return false;
}

void ChessGui::on_cell_drag_begin(GdkDragContext *context, int c, int r){

   int hotx, hoty;

   drag_success=true;  //should be drag_in_progress

   drag_source_c=c;
   drag_source_r=r;

   drag_target_c=drag_source_c;
   drag_target_r=drag_source_r;

   drag_context=Gdk_Drag_Context(context);

   cell_drag_begin.emit(c,r,hot_x, hot_y);

}

void ChessGui::on_cell_drag_end  (GdkDragContext *, int c, int r){

//   cout << "Drag end" << endl;
   if(drag_source_c == drag_target_c && drag_source_r == drag_target_r){
      cell_drag_drop.emit(false,drag_source_c,drag_source_r,drag_target_c,drag_target_r);
   }else{
      cell_drag_drop.emit(true,drag_source_c,drag_source_r,drag_target_c,drag_target_r);
   }

}

void ChessGui::on_cell_drag_leave (GdkDragContext *, guint, int c, int r){
//   cout << "Drag leave" << endl;

   drag_target_c=drag_source_c;
   drag_target_r=drag_source_r;

   //emit signal
   cell_drag_leave.emit(c,r);

}

//void     on_cell_drag_enter (GdkDragContext *, int c, int r);

gboolean ChessGui::on_cell_drag_motion(GdkDragContext *, gint, gint, guint, int c, int r){
//   cout << "Motion in " << c << ", " << r << endl;
   //emit signal
   if(c != drag_target_c || r != drag_target_r){
      cell_drag_enter.emit(c,r);
   }
   cell_drag_motion.emit(c,r);
   drag_target_c=c;
   drag_target_r=r;

   return false;
}

gboolean ChessGui::on_cell_drag_drop (GdkDragContext *, gint, gint, guint, int c, int r){
//   cout << "Drop message" << endl;
   drag_target_c=c;
   drag_target_r=r;
   //synthesize end ev`ent, since we're specifying GDK_DRAG_MOVE
   //and we're not doing the data transfer messages
//   on_cell_drag_end (NULL,c,r);
   return true;

}

gboolean ChessGui::on_delete_event(GdkEventAny*){
   return main_window_close.emit();
}

void ChessGui::main_window_set_size(int width, int height){
   set_usize(width,height);
}

void ChessGui::main_window_set_title(const char *text){
   set_title(text);
}

void ChessGui::main_window_set_active(bool active){
   set_sensitive(active);
}

void ChessGui::main_window_hide(){
   hide();
}

void ChessGui::main_window_show(){
   show();
}


void ChessGui::input_box_clear() {
   filename_entry_box.set_text(string(""));
}

void ChessGui::input_box_set_active(bool active) {
   filename_entry_box.set_sensitive(active);
}

const char *ChessGui::input_box_get_text(){ //maybe make this use the stl string class (and all text)
   return filename_entry_box.get_text().c_str();  //this is not kosher!!!
}

void ChessGui::status_bar_push_text(const char *text){

}

void ChessGui::status_bar_pop_text(){
}

void ChessGui::status_bar_set_text(const char *text){
}

void ChessGui::top_label_set_text(const char *text){
   label1.set_text(text);
}

void ChessGui::bottom_label_set_text(const char *text){
   label2.set_text(text);
}


void ChessGui::button_set_active(int which_button, bool active){
   if(which_button>=MAX_BUTTONS) return;

   button[which_button].set_sensitive(active);

}


void ChessGui::board_cell_color(int c, int r, int color, int layer, int alpha){  //color is 0xRRGGBB, in hex

#ifdef USE_GDK_PIXBUF
	cell_color[c][r][layer]=color;
	cell_color_alpha[c][r][layer]=alpha;
	//g_print("setting color at %d,%d,%d to %d, alpha %d\n",c,r,layer,color, alpha);
#else
   cell_color[c][r]=color;
#endif
   on_cell_configure(NULL,c,r);
   cell[c][r].draw(0);
}

void ChessGui::info_area_clear(){
	info_text.delete_text(0,info_text.get_length());
   //????
}

void ChessGui::info_area_put_text(const char *text,int _fg, int _bg){
   Gdk_GC my_gc;
   Gdk_Color fg;
   Gdk_Color bg;
//   Gdk_Font font;

   my_gc=info_text.get_style()->get_text_gc(info_text.get_state());

   if(_fg>=0){
      fg.set_red  ((_fg & 0x00ff0000) >> 8 | 0x000000ff);
      fg.set_green( _fg & 0x0000ff00 | 0x000000ff);
      fg.set_blue ((_fg & 0x000000ff) << 8 | 0x000000ff);
   }
   if(_bg>=0){
      bg.set_red  ((_bg & 0x00ff0000) >> 8 | 0x000000ff);
      bg.set_green( _bg & 0x0000ff00 | 0x000000ff);
      bg.set_blue ((_bg & 0x000000ff) << 8 | 0x000000ff);
   }
   if(_fg>=0 && _bg >=0){
      info_text.insert(my_gc.get_font(),fg,bg,Gtk::nstring(text),strlen(text));
   }else{
      info_text.insert(string(text));
   }
}

void ChessGui::info_area_put_text(const char *text, const char *font, int _fg, int _bg){


   Gdk_GC my_gc;
   Gdk_Color fg;
   Gdk_Color bg;
   Gdk_Font my_font;

   my_font=Gdk_Font(string(font));

   my_gc=info_text.get_style()->get_text_gc(info_text.get_state());

   if(_fg>=0){
      fg.set_red  ((_fg & 0x00ff0000) >> 8 | 0x000000ff);
      fg.set_green( _fg & 0x0000ff00 | 0x000000ff);
      fg.set_blue ((_fg & 0x000000ff) << 8 | 0x000000ff);
   }
   if(_bg>=0){
      bg.set_red  ((_bg & 0x00ff0000) >> 8 | 0x000000ff);
      bg.set_green( _bg & 0x0000ff00 | 0x000000ff);
      bg.set_blue ((_bg & 0x000000ff) << 8 | 0x000000ff);
   }

   info_text.insert(my_font,fg,bg,Gtk::nstring(text),strlen(text));

}

void ChessGui::board_cell_set_pixmap(int c, int r, const gchar * const *data, int layer, int alpha){

   if(layer >= MAX_LAYERS) return;

#ifdef USE_GDK_PIXBUF
   if(cell_user_pixmap[c][r][layer]){
   	gdk_pixbuf_unref(cell_user_pixmap[c][r][layer]);
	}
	cell_user_pixmap[c][r][layer]=gdk_pixbuf_new_from_xpm_data ((const char **)data);
   cell_user_pixmap_alpha[c][r][layer]=alpha;
		
#else
   if(cell_user_pixmap[c][r][layer]){
      cell_user_pixmap[c][r][layer].release();
      cell_user_bitmap[c][r][layer].release();
   }

   cell_user_pixmap[c][r][layer].create_from_xpm_d(cell[c][r].get_window(),
                                                   cell_user_bitmap[c][r][layer],
                                                   GdkColor(),
                                                   data);
#endif
   cell_user_visible[c][r][layer]=true;

   on_cell_configure(NULL,c,r);
   cell[c][r].draw(0);

}

#ifdef USE_GDK_PIXBUF
void ChessGui::board_cell_set_pixmap(int c, int r, const char *file, int layer, int alpha){
//   g_print("Trying to load %s\n",file);
   if(layer >= MAX_LAYERS) return;

   if(cell_user_pixmap[c][r][layer]){
   	gdk_pixbuf_unref(cell_user_pixmap[c][r][layer]);
	}
	cell_user_pixmap[c][r][layer]=gdk_pixbuf_new_from_file (file);

   cell_user_pixmap_alpha[c][r][layer]=alpha;
		
   cell_user_visible[c][r][layer]=true;

   on_cell_configure(NULL,c,r);
   cell[c][r].draw(0);

}
#endif

void ChessGui::board_cell_clear_pixmap(int c, int r, int layer){  //clears cell

   if(layer >= MAX_LAYERS) return;

   if(layer<0){
      for(int i=0;i<MAX_LAYERS;i++){
#ifdef USE_GDK_PIXBUF
		   if(cell_user_pixmap[c][r][i]){
		   	gdk_pixbuf_unref(cell_user_pixmap[c][r][i]);
        	   cell_user_pixmap[c][r][i]=NULL;
        	}
#else		
         if(cell_user_pixmap[c][r][i]){
            cell_user_pixmap[c][r][i].release();
            cell_user_bitmap[c][r][i].release();
         }
#endif
         cell_user_visible[c][r][i]=false;
      }
   }else{
#ifdef USE_GDK_PIXBUF
	   if(cell_user_pixmap[c][r][layer]){
	   	gdk_pixbuf_unref(cell_user_pixmap[c][r][layer]);
   	   cell_user_pixmap[c][r][layer]=NULL;
		}
#else	
      if(cell_user_pixmap[c][r][layer]){
         cell_user_pixmap[c][r][layer].release();
         cell_user_bitmap[c][r][layer].release();
      }
#endif
      cell_user_visible[c][r][layer]=false;
   }

   on_cell_configure(NULL,c,r);
   cell[c][r].draw(0);


}

void ChessGui::board_cell_pixmap_show(int c, int r, int layer){
   if(layer >= MAX_LAYERS) return;

   if(layer<0){
      for(int i=0;i<MAX_LAYERS;i++){
         cell_user_visible[c][r][i]=true;
      }
   }else{
      cell_user_visible[c][r][layer]=true;
   }

   on_cell_configure(NULL,c,r);
   cell[c][r].draw(0);

}

void ChessGui::board_cell_pixmap_hide(int c, int r, int layer){

   if(layer >= MAX_LAYERS) return;

   if(layer<0){
      for(int i=0;i<MAX_LAYERS;i++){
         cell_user_visible[c][r][i]=false;
      }
   }else{
      cell_user_visible[c][r][layer]=false;
   }

// cout << "cp1" <<endl;
   on_cell_configure(NULL,c,r);
   cell[c][r].draw(0);
// cout << "cp2" <<endl;

}

int  ChessGui::board_cell_get_width(int c, int r){
	return cell[c][r].width();
}

int  ChessGui::board_cell_get_height(int c, int r){
	return cell[c][r].height();
}

void ChessGui::board_cell_set_drag_source(int c, int r){
   cell[c][r].drag_source_set(GDK_BUTTON1_MASK,
                              chess_gui_target_entry,
                              1,
                              GDK_ACTION_COPY);

}

void ChessGui::board_cell_unset_drag_source(int c, int r){
   cell[c][r].drag_source_unset();

}

void ChessGui::board_cell_set_drag_dest(int c, int r){
   cell[c][r].drag_dest_set(GTK_DEST_DEFAULT_ALL,
                            chess_gui_target_entry,
                            1,
                            GDK_ACTION_COPY );

}

void ChessGui::board_cell_unset_drag_dest(int c, int r){
   cell[c][r].drag_dest_unset();

}

void ChessGui::board_cell_set_drag_cursor(int c, int r, int hot_spot_x, int hot_spot_y, const gchar * const *pixmap,int width, int height){
   Gdk_Colormap cm;

   int hotx, hoty;

#ifdef USE_GDK_PIXBUF
	GdkPixbuf *pb=NULL;
	GdkPixbuf *scaled=NULL;
	GdkPixmap *tpm;
	GdkBitmap *tbm;
	
	pb=gdk_pixbuf_new_from_xpm_data((const char **)pixmap);	
	if(width && height){
		scaled=gdk_pixbuf_scale_simple(pb,width,height,GDK_INTERP_BILINEAR);
	   gdk_pixbuf_render_pixmap_and_mask(scaled,
		                                  &tpm,
		                                  &tbm,
		                                  128);
	   if(p) p.release();
	   if(b) b.release();
	   p=Gdk_Pixmap(tpm);
	   b=Gdk_Bitmap(tbm);
	
	
	}else{
	   gdk_pixbuf_render_pixmap_and_mask(pb,
		                                  &tpm,
		                                  &tbm,
		                                  128);
	   if(p) p.release();
	   if(b) b.release();
	   p=Gdk_Pixmap(tpm);
	   b=Gdk_Bitmap(tbm);
	
	}
	
	gdk_pixmap_unref(tpm);
	gdk_bitmap_unref(tbm);
		
	if (pb) gdk_pixbuf_unref(pb);
	if (scaled) gdk_pixbuf_unref(scaled);
	
	//hotspots are not adjusted.
	hotx=hot_spot_x;
	hoty=hot_spot_y;

#else
   if(p) p.release();
   if(b) b.release();
   p.create_from_xpm_d(get_window(),b,Gdk_Color(),pixmap);


   if(width && height) {
	   //Since we can't really scale, adjust the hotpots as if we were
	   //scaling...
		hotx=hot_spot_x-(width-p.width())/2;
		hoty=hot_spot_y-(height-p.height())/2;
   } else {
		//hotspots are not adjusted.
		hotx=hot_spot_x;
		hoty=hot_spot_y;
   }


#endif

   cm=cell[c][r].get_colormap();

   cell[c][r].drag_set_default_icon(cm,
                                   p,
                                   b,
                                   hotx,hoty);

//	cell[c][r].drag_source_set_icon(cm,p,b);

}

#ifdef USE_GDK_PIXBUF
void ChessGui::board_cell_set_drag_cursor(int c, int r, int hot_spot_x, int hot_spot_y, const char *pixmap,int width, int height){
   Gdk_Colormap cm;

   int hotx, hoty;

	GdkPixbuf *pb=NULL;
	GdkPixbuf *scaled=NULL;
	GdkPixmap *tpm;
	GdkBitmap *tbm;
	
	pb=gdk_pixbuf_new_from_file(pixmap);	
	if(width && height){
		scaled=gdk_pixbuf_scale_simple(pb,width,height,GDK_INTERP_BILINEAR);
	   gdk_pixbuf_render_pixmap_and_mask(scaled,
		                                  &tpm,
		                                  &tbm,
		                                  128);
	   if(p) p.release();
	   if(b) b.release();
	   p=Gdk_Pixmap(tpm);
	   b=Gdk_Bitmap(tbm);
	
	
	}else{
	   gdk_pixbuf_render_pixmap_and_mask(pb,
		                                  &tpm,
		                                  &tbm,
		                                  128);
	   if(p) p.release();
	   if(b) b.release();
	   p=Gdk_Pixmap(tpm);
	   b=Gdk_Bitmap(tbm);
	
	}
	
	gdk_pixmap_unref(tpm);
	gdk_bitmap_unref(tbm);
		
	if (pb) gdk_pixbuf_unref(pb);
	if (scaled) gdk_pixbuf_unref(scaled);
	
	//hotspots are not adjusted.
	hotx=hot_spot_x;
	hoty=hot_spot_y;

   cm=cell[c][r].get_colormap();

   cell[c][r].drag_set_default_icon(cm,
                                   p,
                                   b,
                                   hotx,hoty);

//	cell[c][r].drag_source_set_icon(cm,p,b);

}
#endif
void ChessGui::board_cell_unset_drag_cursor(int hot_spot_x, int hot_spot_y, const gchar * const *pixmap){

}


