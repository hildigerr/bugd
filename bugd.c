/* $Id: bugd.c,v 1.9 2013/09/18 01:17:17 moonsdad Exp $ */

/* bugd - A simple Bug Database interface using SQLite and GTK */
#include <stdlib.h>
#include <gtk/gtk.h>
#include <sqlite3.h>

/* ARRAY ACCESS CONSTANTS */
#define H_BOX 0
#define OUTER 0
#define INNER 1
#define MENU 0
#define _BAR 0
#define ROOT 1
#define ITEM 1

/* PREFERENCES */
#define DEFAULT_WINDOW_SIZE 273,373
#define BORDER_WID_OUTER 0
#define BORDER_WID_INNER 8
#define BORDER_WID_TWIXT 4
#define BUG_LIST_COLS 3

/* Global Variables */
sqlite3* bugdb;
int next_keyval;
gboolean opendb = FALSE;

/******************************************************************CALLBACKS: */
void add_bug( gpointer data )
{
     int status = 0;
//     extern int next_keyval;
     gchar* nubug[BUG_LIST_COLS] = { "0", "0", "A Bug" };
//     GtkWidget* button, * table, * field[3];
//
//     GtkWidget* pop_up = gtk_window_new( GTK_WINDOW_POPUP );
//     gtk_window_set_title( GTK_WINDOW(pop_up), "Report a Bug" );
//     //gtk_container_set_border_width( GTK_CONTAINER (pop_up), BORDER_WID_OUTER );
//
//     table = gtk_table_new( 3, 1, TRUE );
//     gtk_container_add( GTK_CONTAINER (pop_up), table );
//
//     button = gtk_button_new_with_label( "Cancel" );
//     g_signal_connect( button, "clicked", G_CALLBACK (add_bug_cancel), NULL );
//     gtk_table_attach_defaults( GTK_TABLE (table[INNER]), button, 0, 1, 1, 2);
//     gtk_widget_show (button);
//
//     gtk_widget_show( table );
//     gtk_widget_show( pop_up );
//
     gtk_clist_append( GTK_CLIST (data), nubug );

    return;
}/* End add_bug Func */
void change_status( gpointer data )
{

    return;
}/* End change_status Func */
void change_display_list( gpointer data )
{

    return;
}/* End change_display_list Func */
void open_reproduce_window( gpointer data )
{

    return;
}/* End open_reproduce_window Func */
void open_behave_window( gpointer data )
{

    return;
}/* End open_behave_window Func */
void event_select( GtkWidget* clist, gint row, gint col, GdkEventButton* event, gpointer data )
{
    gchar* text;

    gtk_clist_get_text(GTK_CLIST(clist), row, 0, &text);

    g_print( "\nSelected Key ID# == %s\n", text );

    return;
}/* End event_select Func */
void file_open_ok( GtkWidget *w, GtkFileSelection *fs )
{
    if( sqlite3_open( gtk_file_selection_get_filename(GTK_FILE_SELECTION (fs)), &bugdb ) )
        fprintf( stderr, "Can't open database: %s\n", sqlite3_errmsg(bugdb));
    else opendb = TRUE;
}/* End file_open_ok Func */
void menu_file_open( void )
{
 	GtkWidget *filew;

    if( opendb ) {
        printf ("A database has already been opened.\n");
    } else {
        filew = gtk_file_selection_new ("Open Bug Database");
        gtk_signal_connect( GTK_OBJECT (filew), "destroy", (GtkSignalFunc) gtk_widget_destroy, GTK_OBJECT (filew));
        gtk_signal_connect_object (GTK_OBJECT (GTK_FILE_SELECTION (filew)->cancel_button),  "clicked", (GtkSignalFunc) gtk_widget_destroy, GTK_OBJECT (filew));
        gtk_signal_connect( GTK_OBJECT (GTK_FILE_SELECTION (filew)->ok_button), "clicked", (GtkSignalFunc) file_open_ok, filew );
        gtk_signal_connect_object (GTK_OBJECT (GTK_FILE_SELECTION (filew)->ok_button),  "clicked", (GtkSignalFunc) gtk_widget_destroy, GTK_OBJECT (filew));
        gtk_widget_show(filew);
    }/* End !opendb Else */
}/* End menu_file_open Func */


/******************************************************************* HELPERS: */
GdkPixbuf* load_pixbuf( const gchar * filename )
{
   GdkPixbuf* pixbuf = NULL;
   GError* error = NULL;

   if( !(pixbuf = gdk_pixbuf_new_from_file( filename, &error )) ) {
      fprintf( stderr, "\nWARNING: %s\n", error->message );
      g_error_free( error );
   }

   return pixbuf;
}/* End load_pixbuf Func */

/********************************************************************** MAIN: */
int main( int argc, char **argv )
{
    GtkWidget* window[2];
    GtkWidget* table[2];
    GtkWidget* menu[2];
    GtkWidget* buglist, * button;
    gchar* buglist_col_titles[BUG_LIST_COLS] = { "ID#", "STATUS", "NAME" };

    gtk_init( &argc, &argv );

    /* Setup Main Window */
    window[OUTER] = gtk_window_new( GTK_WINDOW_TOPLEVEL );
    gtk_window_set_title( GTK_WINDOW (window[OUTER]), "bugd" );
    gtk_window_set_icon( GTK_WINDOW (window[OUTER]), load_pixbuf("icon.png"));
    g_signal_connect( window[OUTER], "destroy", G_CALLBACK (gtk_main_quit), NULL);
    gtk_container_set_border_width( GTK_CONTAINER (window[OUTER]), BORDER_WID_OUTER );
    gtk_window_set_default_size( GTK_WINDOW (window[OUTER]), DEFAULT_WINDOW_SIZE );
    table[OUTER] = gtk_vbox_new (FALSE, 0);
    gtk_container_add( GTK_CONTAINER (window[OUTER]), table[OUTER] );

    /* Setup Menu */
    menu[MENU] = gtk_menu_new();
    menu[ITEM] = gtk_menu_item_new_with_label( "Open..." );
    gtk_menu_shell_append( GTK_MENU_SHELL (menu[MENU]), menu[ITEM] );
    g_signal_connect_swapped( menu[ITEM], "activate", G_CALLBACK (menu_file_open), NULL );
    gtk_widget_show( menu[ITEM] );

    menu[ROOT] = gtk_menu_item_new_with_label( "File" );
    gtk_widget_show( menu[ROOT] );
    gtk_menu_item_set_submenu( GTK_MENU_ITEM (menu[ROOT]), menu[MENU] );

    menu[_BAR] = gtk_menu_bar_new();
    gtk_box_pack_start (GTK_BOX (table[OUTER]), menu[_BAR], FALSE, FALSE, 2);
    gtk_widget_show( menu[_BAR] );

    gtk_menu_shell_append( GTK_MENU_SHELL (menu[_BAR]), menu[ROOT] );

    /* Setup Buglist Window */
    window[INNER] = gtk_scrolled_window_new( NULL, NULL );
    gtk_container_set_border_width( GTK_CONTAINER (window[INNER]), BORDER_WID_INNER );
    gtk_scrolled_window_set_policy( GTK_SCROLLED_WINDOW (window[INNER]), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_box_pack_start( GTK_BOX (table[OUTER]), window[INNER], TRUE, TRUE, 0 );

    buglist = gtk_clist_new_with_titles( BUG_LIST_COLS, buglist_col_titles );
    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW (window[INNER]), buglist );
    gtk_clist_set_column_justification( GTK_CLIST (buglist), 0, GTK_JUSTIFY_CENTER );
    gtk_clist_set_column_justification( GTK_CLIST (buglist), 1, GTK_JUSTIFY_CENTER );
    gtk_signal_connect( GTK_OBJECT(buglist), "select_row", GTK_SIGNAL_FUNC(event_select), NULL );

    /* Setup Button Table */
    table[INNER] = gtk_table_new( 3, 3, TRUE );
    gtk_box_pack_start( GTK_BOX (table[OUTER]), table[INNER], FALSE, TRUE, BORDER_WID_TWIXT );
    gtk_widget_show( table[OUTER] );
    table[H_BOX] = gtk_hbox_new( TRUE, 0 ); /* Reuseing Widget Pointer [H_BOX == OUTER] */

    button = gtk_button_new_with_label( "Add" );
    gtk_signal_connect_object( GTK_OBJECT(button), "clicked", GTK_SIGNAL_FUNC (add_bug), (gpointer) buglist );
    gtk_table_attach_defaults( GTK_TABLE (table[INNER]), button, 0, 1, 0, 1);
    gtk_widget_show (button);

    button = gtk_button_new_with_label( "Fix" );
    g_signal_connect( button, "clicked", G_CALLBACK (change_status), (gpointer) buglist );
    gtk_table_attach_defaults( GTK_TABLE (table[INNER]), button, 1, 2, 0, 1);
    gtk_widget_show (button);

    button = gtk_button_new_with_label( "Hide" );
    g_signal_connect( button, "clicked", G_CALLBACK (change_display_list), (gpointer) buglist );
    gtk_table_attach_defaults( GTK_TABLE (table[INNER]), button, 2, 3, 0, 1);
    gtk_widget_show (button);

    gtk_table_attach_defaults(GTK_TABLE (table[INNER]), table[H_BOX], 0, 3, 1, 2);

    button = gtk_button_new_with_label( "Reproduce" );
    g_signal_connect( button, "clicked", G_CALLBACK (open_reproduce_window), (gpointer) buglist );
    gtk_box_pack_start( GTK_BOX (table[H_BOX]), button, TRUE, TRUE, 0 );
    gtk_widget_show (button);

    button = gtk_button_new_with_label( "Behaviour" );
    g_signal_connect( button, "clicked", G_CALLBACK (open_behave_window), (gpointer) buglist );
    gtk_box_pack_start( GTK_BOX (table[H_BOX]), button, TRUE, TRUE, 0 );
    gtk_widget_show (button);

    button = gtk_button_new_with_label( "Quit" );
    g_signal_connect( button, "clicked", G_CALLBACK (gtk_main_quit), NULL );
    gtk_table_attach_defaults( GTK_TABLE (table[INNER]), button, 1, 2, 2, 3);
    gtk_widget_show (button);

    /* Display GUI */
    gtk_widget_show( buglist );
    gtk_widget_show( table[H_BOX] );
    gtk_widget_show( table[INNER] );
    gtk_widget_show( window[INNER] );
    gtk_widget_show( window[OUTER] );
    gtk_main ();

    if( opendb ) sqlite3_close(bugdb);
    return 0;
}/* End main Func */
