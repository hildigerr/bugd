/* $Id: diagui.c,v 1.7 2013/09/20 04:50:39 moonsdad Exp $ */
#include "bugd.h"


#define DB_FIELD_QT 5

#define BORDER_WID_TEXTF 6
#define DEFAULT_POPUP_SIZE 360,620

/******************************************************************************/
/* Function:   add_bug                                                        */
/* Parameters: gpointer data                                                  */
/******************************************************************************/
void add_bug( gpointer data )//TODO: Change text entry boxes into textbuffers with views
{
    extern gboolean opendb;
    int i;

    //gchar* nubug[BUG_LIST_COLS] = { "0", "0", "A Bug" };
    GtkWidget* box[2], * field[DB_FIELD_QT];
    GtkWidget* pop_up, * button;
    GtkTextBuffer* buffer[DB_FIELD_QT];

    if( !opendb ) { menu_file_open(); return; } /*TODO: Temp: Using "Add" opens db only, if unopen
    //if( !opendb ) return; /* Don't Continue if Failed or Canceled */// Does't continue on success.
    //why == Control reaches here before file is opened.

    pop_up = gtk_window_new( GTK_WINDOW_TOPLEVEL );
    gtk_window_set_title( GTK_WINDOW(pop_up), "Report a Bug" );
    gtk_container_set_border_width( GTK_CONTAINER (pop_up), 12 );
    gtk_window_set_default_size( GTK_WINDOW (pop_up), DEFAULT_POPUP_SIZE );

    box[0] = gtk_vbox_new( FALSE, 0 );
    gtk_container_add( GTK_CONTAINER (pop_up), box[0] );

    for( i = 0; i < DB_FIELD_QT; i++ ) {
        field[i] = gtk_text_view_new();
        buffer[i] = gtk_text_view_get_buffer( GTK_TEXT_VIEW (field[i]) );
    }

    box[1] = gtk_hbox_new( FALSE, 0 );
    gtk_box_pack_start( GTK_BOX (box[0]), box[1] , FALSE, FALSE, BORDER_WID_TEXTF);
    button = gtk_label_new("NAME: "); /* Not A Button */
    gtk_box_pack_start( GTK_BOX (box[1]), button, FALSE, FALSE, BORDER_WID_TEXTF);
    gtk_box_pack_start( GTK_BOX (box[1]), field[0], TRUE, TRUE, BORDER_WID_TEXTF);
    gtk_widget_show( button );
    gtk_widget_show( box[1] );

    box[1] = gtk_frame_new( "Expectation:" );//This is where you put a description of what you expect to be happening.
    gtk_box_pack_start( GTK_BOX (box[0]), box[1] , TRUE, TRUE, BORDER_WID_TEXTF);
    gtk_container_add( GTK_CONTAINER (box[1]), field[1]);
    gtk_widget_show( box[1] );

    box[1] = gtk_frame_new( "Behavior:" );//Describe what is actually happening.
    gtk_box_pack_start( GTK_BOX (box[0]), box[1] , TRUE, TRUE, BORDER_WID_TEXTF);
    gtk_container_add( GTK_CONTAINER (box[1]), field[2]);
    gtk_widget_show( box[1] );

    box[1] = gtk_frame_new( "Reproduce:" );//Provide instructions for reproducing the issue.
    gtk_box_pack_start( GTK_BOX (box[0]), box[1] , TRUE, TRUE, BORDER_WID_TEXTF);
    gtk_container_add( GTK_CONTAINER (box[1]), field[3]);
    gtk_widget_show( box[1] );

    box[1] = gtk_frame_new( "Notes:" );//This is a workspace. Eg: Who is assigned to work on this problem
    gtk_box_pack_start( GTK_BOX (box[0]), box[1] , TRUE, TRUE, BORDER_WID_TEXTF);
    gtk_container_add( GTK_CONTAINER (box[1]), field[4]);
    gtk_widget_show( box[1] );

    box[1] = gtk_hbox_new( FALSE, 0 );
    gtk_box_pack_start( GTK_BOX (box[0]), box[1] , FALSE, FALSE, BORDER_WID_TEXTF);
    button = gtk_button_new_with_label( "Submit" );
    //gtk_signal_connect( GTK_OBJECT (button), "clicked", (GtkSignalFunc) submit_bug, GTK_OBJECT (field) );
    gtk_box_pack_end( GTK_BOX (box[1]), button , FALSE, FALSE, BORDER_WID_TEXTF);
    gtk_widget_show( button );
    button = gtk_button_new_with_label( "Cancel" );
    gtk_signal_connect( GTK_OBJECT (button), "clicked", (GtkSignalFunc) close_window, GTK_OBJECT (pop_up) );
    gtk_box_pack_end( GTK_BOX (box[1]), button , FALSE, FALSE, BORDER_WID_TEXTF);
    gtk_widget_show( button );

    gtk_widget_show( box[1] );

    for( i = 0; i < DB_FIELD_QT; i++ ) gtk_widget_show( field[i] );
    gtk_widget_show( box[0] );
    gtk_widget_show( pop_up );

    //gtk_clist_append( GTK_CLIST (data), nubug );

}/* End add_bug Func */


/******************************************************************************/
/* Function:   change_display_list                                            */
/* Parameters: gpointer data*/
/* WARNING: */
/******************************************************************************/
void change_display_list( gpointer data )
{

    return;
}/* End change_display_list Func */


/******************************************************************************/
/* Function:   open_reproduce_window                                          */
/* Parameters: gpointer data*/
/* WARNING: */
/******************************************************************************/
void open_reproduce_window( gpointer data )
{

    return;
}/* End open_reproduce_window Func */


/******************************************************************************/
/* Function:   open_behave_window                                             */
/* Parameters: gpointer data*/
/* WARNING: */
/******************************************************************************/
void open_behave_window( gpointer data )
{

    return;
}/* End open_behave_window Func */


/******************************************************************* HELPERS: */
/******************************************************************************/
/* Function:    load_pixbuf                                                   */
/* Parameters:  const gchar ptr filename */
/* Returns:     GdkPixbuf ptr */
/* WARNING: On Fail, just uses default img and displays warning in terminal.  */
/******************************************************************************/
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

