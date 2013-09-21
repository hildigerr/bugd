/* $Id: menu.c,v 1.7 2013/09/21 01:56:57 moonsdad Exp $ */
#include "bugd.h"


/******************************************************************************/
/* Function:   menu_file_open                                                 */
/* WARNING: */
/******************************************************************************/
void menu_file_open( void )
{
 	GtkWidget *filew;
    extern gboolean opendb;

    if( opendb ) { //TODO: Popup - Verify - Close
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
void file_open_ok( GtkWidget *w, GtkFileSelection *fs )
{
    char* statement = "SELECT * FROM bug_list";

    extern sqlite3* bugdb;
    extern gboolean opendb;
    char* errmsg;

    if( sqlite3_open( gtk_file_selection_get_filename(GTK_FILE_SELECTION (fs)), &bugdb ) )
        fprintf( stderr, "Can't open database: %s\n", sqlite3_errmsg(bugdb));
    else opendb = TRUE;

    sqlite3_exec( bugdb, statement, load_open_datab, NULL, &errmsg );

    if( errmsg ) {
        fprintf( stderr, "\n%s\nCreating new table...", errmsg );
        sqlite3_exec( bugdb, "create table bug_list( Id INTEGER PRIMARY KEY, Name TEXT, Reproduce TEXT, Expectation TEXT, Behavior TEXT, Notes TEXT, Status INTEGER )", NULL, NULL, &errmsg );
        if( errmsg ) fprintf( stderr, "\n%s\n", errmsg );
    }
}/* End file_open_ok Func */


/******************************************************************************/
/* Function:   menu_file_close                                                */
/* WARNING: */
/******************************************************************************/
void menu_file_close( void )
{
    extern sqlite3* bugdb;
    extern gboolean opendb;
    extern GtkListStore* buglist;

    if( opendb ) sqlite3_close(bugdb);
    else printf("\nNo database open.\n");
    opendb = FALSE;
    gtk_list_store_clear( buglist );
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
