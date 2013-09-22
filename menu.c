/* $Id: menu.c,v 1.11 2013/09/22 05:15:11 moonsdad Exp $ */
#include "bugd.h"


/******************************************************************************/
/* Function:   menu_file_open                                                 */
/* Allows user to search filesystem for the database to open/create.          */
/* Currently, does nothing when a database is already open.                   */
/* TODO:    -Popup-Verify-Close when db is already open                       */
/*          -hide files based on extension filter.                            */
/******************************************************************************/
void menu_file_open( void )
{
 	GtkWidget *filew;
    extern gboolean opendb;

    if( opendb ) {
        g_print( "\nA database has already been opened.\n" );
    } else {
        filew = gtk_file_selection_new( "Open Bug Database" );
        gtk_file_selection_complete(  GTK_FILE_SELECTION (filew), "*.db" );
        gtk_signal_connect( GTK_OBJECT (filew), "destroy", (GtkSignalFunc) gtk_widget_destroy, GTK_OBJECT (filew));
        gtk_signal_connect_object (GTK_OBJECT (GTK_FILE_SELECTION (filew)->cancel_button),  "clicked", (GtkSignalFunc) gtk_widget_destroy, GTK_OBJECT (filew));
        gtk_signal_connect( GTK_OBJECT (GTK_FILE_SELECTION (filew)->ok_button), "clicked", (GtkSignalFunc) file_open_ok, filew );
        gtk_signal_connect_object (GTK_OBJECT (GTK_FILE_SELECTION (filew)->ok_button),  "clicked", (GtkSignalFunc) gtk_widget_destroy, GTK_OBJECT (filew));
        gtk_widget_show(filew);
    }/* End !opendb Else */
}/* End menu_file_open Func */


/******************************************************************************/
/* Function:    file_open_ok                                                  */
/* Parameters:  GtkWidget ptr           w                                     */
/*              GtkFileSelection ptr    fs                                    */
/* Called by the file selection dialogue box when the ok button is clicked.   */
/******************************************************************************/
void file_open_ok( GtkWidget *w, GtkFileSelection *fs )
{
    char* statement = "SELECT Id, Status, Name FROM bug_list";

    extern sqlite3* bugdb;
    extern gboolean opendb;
    char* errmsg;

    if( sqlite3_open( gtk_file_selection_get_filename(GTK_FILE_SELECTION (fs)), &bugdb ) ) {
        fprintf( stderr, "Can't open database: %s\n", sqlite3_errmsg(bugdb));
        return;
    } else opendb = TRUE;

    sqlite3_exec( bugdb, statement, load_open_datab, NULL, &errmsg );

    if( errmsg ) {
        fprintf( stderr, "\nWARNING: %s. Creating new table...", errmsg );
        sqlite3_free( errmsg );
        sqlite3_exec( bugdb, "create table bug_list( Id INTEGER PRIMARY KEY, Name TEXT, Reproduce TEXT, Expectation TEXT, Behavior TEXT, Notes TEXT, Status INTEGER )", NULL, NULL, &errmsg );
        if( errmsg ) {
            fprintf( stderr, "\n%s\n", errmsg );
            sqlite3_free( errmsg );
        }
    }
}/* End file_open_ok Func */


/******************************************************************************/
/* Function:   menu_file_close                                                */
/******************************************************************************/
void menu_file_close( void )
{
    extern sqlite3* bugdb;
    extern gboolean opendb;
    extern GtkListStore* buglist;

    if( opendb ) sqlite3_close( bugdb );
    else printf( "\nNo database open.\n" );
    opendb = FALSE;
    gtk_list_store_clear( buglist );
}/* End menu_file_close Func */


/******************************************************************************/
/* Function:    close_window                                                  */
/* Parameters:  GtkWidget ptr   widget  -- The Parent widget                  */
/*              gpointer        window  -- The window to destroy              */
/******************************************************************************/
void close_window( GtkWidget *widget, gpointer window )
{
    gtk_widget_destroy(GTK_WIDGET(window));
}/* End close_window Func */
