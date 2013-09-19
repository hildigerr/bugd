/* $Id: diagui.c,v 1.1 2013/09/19 03:48:12 moonsdad Exp $ */
#include "bugd.h"

/* Global Variables */
extern sqlite3* bugdb;
extern gboolean opendb;


void add_bug( gpointer data )//TODO: Change text entry boxes into textbuffers with views
{
    int i, status = 0;
    extern int next_keyval;
    gchar* nubug[BUG_LIST_COLS] = { "0", "0", "A Bug" };
    GtkWidget* box[2], * field[3];
    GtkWidget* pop_up, * button;

    if( !opendb ) { menu_file_open(); return; } /*TODO: Temp: Using "Add" opens db only, if unopen
    //if( !opendb ) return; /* Don't Continue if Failed or Canceled */// Does't continue on success.
    //why == Control reaches here before file is opened.

    pop_up = gtk_window_new( GTK_WINDOW_TOPLEVEL );
    gtk_window_set_title( GTK_WINDOW(pop_up), "Report a Bug" );
    gtk_container_set_border_width( GTK_CONTAINER (pop_up), 0 );

    box[0] = gtk_vbox_new( FALSE, 0 );
    gtk_container_add( GTK_CONTAINER (pop_up), box[0] );

    for( i = 0; i < 3; i++ ) field[i] = gtk_entry_new( );

    box[1] = gtk_hbox_new( FALSE, 0 );
    gtk_box_pack_start( GTK_BOX (box[0]), box[1] , FALSE, FALSE, BORDER_WID_TWIXT);
    button = gtk_label_new("NAME: "); /* Not A Button */
    gtk_box_pack_start( GTK_BOX (box[1]), button, FALSE, FALSE, BORDER_WID_TWIXT);
    gtk_box_pack_start( GTK_BOX (box[1]), field[0], TRUE, TRUE, BORDER_WID_TWIXT);
    gtk_widget_show( button );
    gtk_widget_show( box[1] );

    box[1] = gtk_frame_new( "Expectation:" );
    gtk_box_pack_start( GTK_BOX (box[0]), box[1] , TRUE, TRUE, BORDER_WID_TWIXT);
    gtk_container_add( GTK_CONTAINER (box[1]), field[1]);
    gtk_widget_show( box[1] );

    box[1] = gtk_frame_new( "Behavior:" );
    gtk_box_pack_start( GTK_BOX (box[0]), box[1] , TRUE, TRUE, BORDER_WID_TWIXT);
    gtk_container_add( GTK_CONTAINER (box[1]), field[2]);
    gtk_widget_show( box[1] );

//TRING TO MAKE TEXTBOX FILL FRAME:
//     box[1] = gtk_frame_new( "Behavior:" );
//     gtk_box_pack_start( GTK_BOX (box[0]), box[1] , TRUE, TRUE, BORDER_WID_TWIXT);
//     button = gtk_table_new( 1,1,TRUE ); /* Not A Button */
//     gtk_container_add( GTK_CONTAINER (box[1]), button );
//     gtk_table_attach_defaults( GTK_TABLE (button), field[2],0,1,0,1 );
//     gtk_widget_show( button );
//     gtk_widget_show( box[1] );

    box[1] = gtk_hbox_new( FALSE, 0 );
    gtk_box_pack_start( GTK_BOX (box[0]), box[1] , FALSE, FALSE, BORDER_WID_TWIXT);
    button = gtk_button_new_with_label( "Submit" );
    //gtk_signal_connect( GTK_OBJECT (button), "clicked", (GtkSignalFunc) submit_bug, GTK_OBJECT (field) );
    gtk_box_pack_end( GTK_BOX (box[1]), button , FALSE, FALSE, BORDER_WID_TWIXT);
    gtk_widget_show( button );
    button = gtk_button_new_with_label( "Cancel" );
    gtk_signal_connect( GTK_OBJECT (button), "clicked", (GtkSignalFunc) close_window, GTK_OBJECT (pop_up) );
    gtk_box_pack_end( GTK_BOX (box[1]), button , FALSE, FALSE, BORDER_WID_TWIXT);
    gtk_widget_show( button );

    gtk_widget_show( box[1] );

    for( i = 0; i < 3; i++ ) gtk_widget_show( field[i] );
    gtk_widget_show( box[0] );
    gtk_widget_show( pop_up );

    gtk_clist_append( GTK_CLIST (data), nubug );

}/* End add_bug Func */

/***********************************************************************TODO: */
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

