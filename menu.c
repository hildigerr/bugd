/* $Id: menu.c,v 1.1 2013/09/19 03:48:12 moonsdad Exp $ */
#include "bugd.h"

/* Global Variables */
extern sqlite3* bugdb;
extern gboolean opendb;


void menu_file_open( void )//TODO: Verify valid selection; display w/ extension filter
{
 	GtkWidget *filew;

    if( opendb ) {
        printf("\nA database has already been opened.\n");
    } else {
        filew = gtk_file_selection_new ("Open Bug Database");
        gtk_signal_connect( GTK_OBJECT (filew), "destroy", (GtkSignalFunc) gtk_widget_destroy, GTK_OBJECT (filew));
        gtk_signal_connect_object (GTK_OBJECT (GTK_FILE_SELECTION (filew)->cancel_button),  "clicked", (GtkSignalFunc) gtk_widget_destroy, GTK_OBJECT (filew));
        gtk_signal_connect( GTK_OBJECT (GTK_FILE_SELECTION (filew)->ok_button), "clicked", (GtkSignalFunc) file_open_ok, filew );
        gtk_signal_connect_object (GTK_OBJECT (GTK_FILE_SELECTION (filew)->ok_button),  "clicked", (GtkSignalFunc) gtk_widget_destroy, GTK_OBJECT (filew));
        gtk_widget_show(filew);
    }/* End !opendb Else */
}/* End menu_file_open Func */



void file_open_ok( GtkWidget *w, GtkFileSelection *fs )
{
    extern int next_keyval; //TODO Get and Set after Opendb

    if( sqlite3_open( gtk_file_selection_get_filename(GTK_FILE_SELECTION (fs)), &bugdb ) )
        fprintf( stderr, "Can't open database: %s\n", sqlite3_errmsg(bugdb));
    else opendb = TRUE;
}/* End file_open_ok Func */


void menu_file_close( void )
{
    if( opendb ) sqlite3_close(bugdb);
    else printf("\nNo database open.\n");
    opendb = FALSE;
}/* End menu_file_close Func */



void close_window(GtkWidget *widget, gpointer window)
{
    gtk_widget_destroy(GTK_WIDGET(window));
}
