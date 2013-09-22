/* $Id: bugd.h,v 1.11 2013/09/22 01:40:08 moonsdad Exp $ */
#pragma once

#include <stdlib.h>
#include <gtk/gtk.h>
#include <sqlite3.h>

//#define DEBUG

#define DB_FIELD_QT 5

/* PREFERENCES */
#define DEFAULT_WINDOW_SIZE 273,373
#define BORDER_WID_INNER 8
#define BORDER_WID_TWIXT 4
#define BUG_LIST_COLS 3

typedef struct { GtkWidget* field[DB_FIELD_QT]; } FIELD_LIST;

enum { ID_COL, STATUS_COL, NAME_COL, LIST_COL_QT };

/* Main Button Callbacks */
void add_bug( void );
void change_status( gpointer b, gpointer data );
void modify_select_bug( gpointer b, gpointer data );
void open_reproduce_window( gpointer b, gpointer data );
void open_behave_window( gpointer b, gpointer data );

/* Menu Callbacks */
void menu_file_open( void );
void menu_file_close( void );

/* Dialoge Box Buttons */
void close_window(GtkWidget *widget, gpointer window);
void file_open_ok( GtkWidget *w, GtkFileSelection *fs );
void submit_bug( void );

/* Bug List Store Functions */
GtkWidget* init_list( void );
int load_open_datab( void* pArg, int argc, char** argv, char** columnNames );

/* Helpers and Wrappers */
GdkPixbuf* load_pixbuf( const gchar * filename );
