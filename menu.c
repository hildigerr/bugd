/* $Id: menu.c,v 1.4 2013/09/19 06:04:17 moonsdad Exp $ */
#include "bugd.h"


/******************************************************************************/
/* Function:   menu_file_open                                                 */
/* WARNING: */
/******************************************************************************/
void menu_file_open( void )
{
 	GtkWidget *filew;
    extern gboolean opendb;

    if( opendb ) {
        printf("\nA database has already been opened.\n");
    } else {
        filew = gtk_file_selection_new( "Open Bug Database" );
        gtk_file_selection_complete(  GTK_FILE_SELECTION (filew), "*.db" );//TODO: hide files based on extension filter
        gtk_signal_connect( GTK_OBJECT (filew), "destroy", (GtkSignalFunc) gtk_widget_destroy, GTK_OBJECT (filew));
        gtk_signal_connect_object (GTK_OBJECT (GTK_FILE_SELECTION (filew)->cancel_button),  "clicked", (GtkSignalFunc) gtk_widget_destroy, GTK_OBJECT (filew));
        gtk_signal_connect( GTK_OBJECT (GTK_FILE_SELECTION (filew)->ok_button), "clicked", (GtkSignalFunc) file_open_ok, filew );
        gtk_signal_connect_object (GTK_OBJECT (GTK_FILE_SELECTION (filew)->ok_button),  "clicked", (GtkSignalFunc) gtk_widget_destroy, GTK_OBJECT (filew));
        gtk_widget_show(filew);
    }/* End !opendb Else */
}/* End menu_file_open Func */


/******************************************************************************/
/* Function:   file_open_ok                                                   */
/* Parameters: GtkWidget ptr w, GtkFileSelection ptr fs */
/* WARNING: */
/******************************************************************************/
void file_open_ok( GtkWidget *w, GtkFileSelection *fs )//TODO: Verify valid selection
{
    extern unsigned next_keyval; //TODO Get and Set after Opendb
    extern sqlite3* bugdb;
    extern gboolean opendb;

    if( sqlite3_open( gtk_file_selection_get_filename(GTK_FILE_SELECTION (fs)), &bugdb ) )
        fprintf( stderr, "Can't open database: %s\n", sqlite3_errmsg(bugdb));
    else opendb = TRUE;
}/* End file_open_ok Func */


/******************************************************************************/
/* Function:   menu_file_close                                                */
/* WARNING: */
/******************************************************************************/
void menu_file_close( void )
{
    extern sqlite3* bugdb;
    extern gboolean opendb;

    if( opendb ) sqlite3_close(bugdb);
    else printf("\nNo database open.\n");
    opendb = FALSE;
}/* End menu_file_close Func */


/******************************************************************************/
/* Function:   close_window                                                   */
/* Parameters: GtkWidget ptr widget, gpointer window */
/* WARNING: */
/******************************************************************************/
void close_window(GtkWidget *widget, gpointer window)
{
    gtk_widget_destroy(GTK_WIDGET(window));
}/* End close_window Func */
