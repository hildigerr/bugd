/* $Id: bugd.h,v 1.5 2013/09/21 04:21:05 moonsdad Exp $ */
#pragma once

#include <stdlib.h>
#include <gtk/gtk.h>
#include <sqlite3.h>

#define DB_FIELD_QT 5

/* PREFERENCES */
#define DEFAULT_WINDOW_SIZE 273,373
#define BORDER_WID_INNER 8
#define BORDER_WID_TWIXT 4
#define BUG_LIST_COLS 3

typedef struct { GtkWidget* field[DB_FIELD_QT]; } FIELD_LIST;

enum { ID_COL, STATUS_COL, NAME_COL, LIST_COL_QT };

/* Main Button Callbacks */
void add_bug( gpointer data );
void change_status( gpointer data );
void change_display_list( gpointer data );
void open_reproduce_window( gpointer data );
void open_behave_window( gpointer data );

/* Menu Callbacks */
void menu_file_open( void );
void menu_file_close( void );

/* Dialoge Box Buttons */
void close_window(GtkWidget *widget, gpointer window);
void file_open_ok( GtkWidget *w, GtkFileSelection *fs );
void submit_bug( );//FIELD_LIST* my );

/* Bug List Store Functions */
void event_select( GtkWidget* clist, gint row, gint col, GdkEventButton* event, gpointer data );
GtkWidget* init_list( void );
int load_open_datab( void* pArg, int argc, char** argv, char** columnNames );

/* Helpers and Wrappers */
GdkPixbuf* load_pixbuf( const gchar * filename );
