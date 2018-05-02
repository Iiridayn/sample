#ifndef __CHESSGUI_H__
#define __CHESSGUI_H__

#include <sigc++/signal_system.h>

using namespace std;

//using namespace SigC;



#include <gtk--/base.h>
#include <gtk--/main.h>
#include <gtk--/style.h>
#include <gtk--/window.h>
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
#include <gtk--/aspectframe.h>

#ifdef USE_GDK_PIXBUF
#include <gdk-pixbuf/gdk-pixbuf.h>

#endif

#define MAX_BUTTONS       4
#define MAX_BOARD_COLUMNS 8
#define MAX_BOARD_ROWS    8
#define MAX_LAYERS        5
#define MAX_LAYER_VALUE   4
#define DEFAULT_FILENAME_ENTRY_LABEL "Filename:"

#define CHESS_GUI_DND_TARGET "//byuchessgui//"
#define CHESS_GUI_DND_INFO 0xdeadbeef

//enum {LOAD_BUTTON, SAVE_BUTTON, UNDO_BUTTON, DEBUG_BUTTON} GUI_BUTTONS;

class ChessGui:public Gtk::Window {
protected:

	//internal private variables and gtk+ widgets
   bool drag_success;
   int  drag_target_c;
   int  drag_target_r;
   int  drag_source_c;
   int  drag_source_r;
   int hot_x;
   int hot_y;

   Gdk_Drag_Context drag_context;

   Gdk_Pixmap p;
   Gdk_Bitmap b;
   GtkTargetEntry chess_gui_target_entry[1];

   Gtk::VBox main_vbox;
   Gtk::Statusbar status_bar;
   Gtk::HBox hbox;

   Gtk::VBox left_pane;
   Gtk::Label label1;  //maybe change this to a text widget for fonts, etc
   Gtk::AspectFrame *board_frame;
   Gtk::Table board_table;
//   Gtk::AspectFrame table_aspect_frame(0.5,0.5,1,false);
   Gtk::DrawingArea cell[MAX_BOARD_COLUMNS][MAX_BOARD_ROWS];
#ifdef USE_GDK_PIXBUF
	GdkPixbuf *cell_backing_store[MAX_BOARD_COLUMNS][MAX_BOARD_ROWS];
#else
   Gdk_Pixmap cell_backing_store[MAX_BOARD_COLUMNS][MAX_BOARD_ROWS];
#endif


#ifdef USE_GDK_PIXBUF
	GdkPixbuf *cell_user_pixmap[MAX_BOARD_COLUMNS][MAX_BOARD_ROWS][MAX_LAYERS];
	int        cell_user_pixmap_alpha[MAX_BOARD_COLUMNS][MAX_BOARD_ROWS][MAX_LAYERS];
   int cell_color[MAX_BOARD_COLUMNS][MAX_BOARD_ROWS][MAX_LAYERS];
   int cell_color_alpha[MAX_BOARD_COLUMNS][MAX_BOARD_ROWS][MAX_LAYERS];
#else
   Gdk_Pixmap cell_user_pixmap[MAX_BOARD_COLUMNS][MAX_BOARD_ROWS][MAX_LAYERS];
   Gdk_Bitmap cell_user_bitmap[MAX_BOARD_COLUMNS][MAX_BOARD_ROWS][MAX_LAYERS];
   int cell_color[MAX_BOARD_COLUMNS][MAX_BOARD_ROWS];
#endif
   bool cell_user_visible[MAX_BOARD_COLUMNS][MAX_BOARD_ROWS][MAX_LAYERS];


   Gtk::Label label2;  //maybe change this to a text widget for fonts, etc
   Gtk::HSeparator separator;

   Gtk::HBox filename_entry_hbox;
   Gtk::Label filename_entry_label;
   Gtk::Entry filename_entry_box;

   Gtk::VBox right_pane;
   Gtk::Frame button_frame;
   Gtk::VBox  button_vbox;
   Gtk::Button button[4];  //initialize 4 buttons
   Gtk::Frame info_frame;
   Gtk::ScrolledWindow info_scrolled_window;
   Gtk::Text info_text;



   //private, local event callbacks:

//   void on_button_clicked();
   void on_button_clicked(int which_button);
   gint on_mouse_motion(GdkEventMotion *,int c, int r);
   gint on_mouse_button_released(GdkEventButton *, int c, int r);
   gint on_mouse_button_pressed(GdkEventButton *, int c, int r);
   gint on_cell_expose(GdkEventExpose *, int c, int r);
   gint on_cell_configure(GdkEventConfigure *, int c, int r);

   void     on_cell_drag_begin(GdkDragContext *, int c, int r);
   void     on_cell_drag_end  (GdkDragContext *, int c, int r);
   void     on_cell_drag_leave (GdkDragContext *, guint, int c, int r);
// void     on_cell_drag_enter (GdkDragContext *, int c, int r);
   gboolean on_cell_drag_motion(GdkDragContext *, gint, gint, guint, int c, int r);
   gboolean on_cell_drag_drop         (GdkDragContext *, gint, gint, guint, int c, int r);
   gint on_delete_event(GdkEventAny *);


public:
   ChessGui();
   ~ChessGui();

   //signals

   //resize signals??

   //user must capture the destroy event!!!
   //signal int main_window_close ()
   SigC::Signal0<int> main_window_close;

   //signal void cell_resized (int width, int height)
   SigC::Signal2<void,int,int> cell_resized;

   //signal void button_clicked(int which_button)
   SigC::Signal1<void,int> button_clicked;

   //signal void mouse_button_released(int c, int r, int button);
   //button: 1=left, 2=right, 3=middle, etc
   SigC::Signal3<void,int,int,int> mouse_button_released;

   //signal void mouse_button_pressed(int c, int r, int type,int button);
   //type: 1=normal click, 2=double click, 3=triple click
   //button: 1=left, 2=middle, 3=right, etc
   SigC::Signal4<void,int,int,int,int> mouse_button_pressed;

   //signal void mouse_motion(int c, int r)
   SigC::Signal2<void,int,int> mouse_motion;

   //signal void drag_begin (int c, int r, int hot_x, int hot_y);
   SigC::Signal4 <void,int,int,int,int> cell_drag_begin;

   //signal void cell_drag_enter (int c, int r)  //this one is synthesized
   SigC::Signal2<void,int,int> cell_drag_enter;

   //signal void cell_drag_leave (int c, int r)
   SigC::Signal2<void,int,int> cell_drag_leave;

   //signal void cell_drag_motion (int c, int r)
   SigC::Signal2<void,int,int> cell_drag_motion;

   //signal void drag_drop (bool success, int src_x, int src_y, int dst_x, int dst_y);
   SigC::Signal5<void,bool,int,int,int,int> cell_drag_drop;

   //gui control methods
   //  change these all to string stl objects!
   void main_window_set_size(int width, int height);
   void main_window_set_title(const char *text);
   void main_window_set_active(bool active);
   void main_window_hide();  //use the inherited show() and hide() methods
   void main_window_show();

   void input_box_clear();
   void input_box_set_active(bool active);
   const char *input_box_get_text(); //maybe make this use the stl string class (and all text)

   void status_bar_push_text(const char *text);
   void status_bar_pop_text();
   void status_bar_set_text(const char *text);

   void top_label_set_text(const char *text);
   void bottom_label_set_text(const char *text);

   void button_set_active(int which_button, bool active);
//   void button_set_text(int which_button, const char *text);

   void info_area_clear();
   void info_area_put_text(const char *text,int fg=-1, int bg=-1);
   void info_area_put_text(const char *text, const char *font, int fg, int bg);

   // if gdk_pixbuf is not used, color is alays applied, solid to the background of the cell
   //    in this case subsequent tiles will paint over it.
   void board_cell_color(int column, int row, int color,int layer=MAX_LAYER_VALUE,int alpha=255);  //color is 0xRRGGBB, in hex
   void board_cell_set_pixmap  (int c, int r, const gchar * const *data, int layer=0, int alpha=255);
#ifdef USE_GDK_PIXBUF
   void board_cell_set_pixmap  (int c, int r, const char *filename, int layer=0, int alpha=255);
#endif
   void board_cell_clear_pixmap(int c, int r, int layer=-1);  //clears cell
   void board_cell_pixmap_show(int c, int r, int layer=-1);
   void board_cell_pixmap_hide(int c, int r, int layer=-1);
   int  board_cell_get_width(int c, int r);
   int  board_cell_get_height(int c, int r);

   void board_cell_set_drag_source  (int column, int row);
   void board_cell_unset_drag_source(int column, int row);
   void board_cell_set_drag_dest  (int column, int row);
   void board_cell_unset_drag_dest(int column, int row);
   void board_cell_set_drag_cursor  (int source_c, int source_r,
                                     int hot_spot_x, int hot_spot_y,
                                     const gchar * const *pixmap,
                                     int width=0, int height=0);
#ifdef USE_GDK_PIXBUF
   void board_cell_set_drag_cursor  (int source_c, int source_r,
                                     int hot_spot_x, int hot_spot_y,
                                     const char *pixmap,
                                     int width=0, int height=0);
#endif
   void board_cell_unset_drag_cursor(int hot_spot_x, int hot_spot_y, const gchar * const *pixmap);

};


#endif
