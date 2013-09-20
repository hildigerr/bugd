/* $Id: bugd.h,v 1.2 2013/09/20 02:26:10 moonsdad Exp $ */
#pragma once

#include <stdlib.h>
#include <gtk/gtk.h>
#include <sqlite3.h>

/* PREFERENCES */// TODO: Dynamize
#define DEFAULT_WINDOW_SIZE 273,373
//#define BORDER_WID_OUTER 0
#define BORDER_WID_INNER 8
#define BORDER_WID_TWIXT 4
#define BUG_LIST_COLS 3

/* Main Button Callbacks */
void add_bug( gpointer data );
void change_status( gpointer data );
void change_display_list( gpointer data );
void open_reproduce_window( gpointer data );
void open_behave_window( gpointer data );

/* Events */
void event_select( GtkWidget* clist, gint row, gint col, GdkEventButton* event, gpointer data );

/* Menu Callbacks */
void menu_file_open( void );
void menu_file_close( void );

/* Dialoge Box Buttons */
void close_window(GtkWidget *widget, gpointer window);
void file_open_ok( GtkWidget *w, GtkFileSelection *fs );

/* Helpers and Wrappers */
GdkPixbuf* load_pixbuf( const gchar * filename );


int load_open_datab( void* pArg, int argc, char** argv, char** columnNames );
