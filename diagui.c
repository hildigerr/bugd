/* $Id: diagui.c,v 1.21 2013/09/22 05:15:10 moonsdad Exp $ */
#include "bugd.h"

#define BORDER_WID_TEXTF 6
#define DEFAULT_POPUP_SIZE 360,620

/******************************************************************************/
/* Function:    add_bug                                                       */
/* Parameters:  gpointer data                                                 */
/* WARNING: Only one instance of this open at a time. TODO: Enforce           */
/******************************************************************************/
void add_bug( void )
{
    extern FIELD_LIST fl;
    extern gboolean opendb;
    GtkWidget* box[2];
    GtkWidget* pop_up, * button;
    int i;

    if( !opendb ) { menu_file_open(); return; } //TODO: Temp: Using "Add" opens db only, if unopen
    //if( !opendb ) return; /* Don't Continue if Failed or Canceled */// Does't continue on success.
    //why == Control reaches here before file is opened.

    for( i = 0; i < DB_FIELD_QT; i++ ) fl.field[i] = gtk_text_view_new();

    pop_up = gtk_window_new( GTK_WINDOW_TOPLEVEL );
    gtk_window_set_title( GTK_WINDOW(pop_up), "Report a Bug" );
    gtk_container_set_border_width( GTK_CONTAINER (pop_up), 12 );
    gtk_window_set_default_size( GTK_WINDOW (pop_up), DEFAULT_POPUP_SIZE );

    box[0] = gtk_vbox_new( FALSE, 0 );
    gtk_container_add( GTK_CONTAINER (pop_up), box[0] );

    box[1] = gtk_hbox_new( FALSE, 0 );
    gtk_box_pack_start( GTK_BOX (box[0]), box[1] , FALSE, FALSE, BORDER_WID_TEXTF);
    button = gtk_label_new("NAME: "); /* Not A Button */
    gtk_box_pack_start( GTK_BOX (box[1]), button, FALSE, FALSE, BORDER_WID_TEXTF);
    gtk_box_pack_start( GTK_BOX (box[1]), fl.field[0], TRUE, TRUE, BORDER_WID_TEXTF);
    gtk_widget_show( button );
    gtk_widget_show( box[1] );

    box[1] = gtk_frame_new( "Expectation:" );//This is where you put a description of what you expect to be happening.
    gtk_box_pack_start( GTK_BOX (box[0]), box[1] , TRUE, TRUE, BORDER_WID_TEXTF);
    gtk_container_add( GTK_CONTAINER (box[1]), fl.field[1]);
    gtk_widget_show( box[1] );

    box[1] = gtk_frame_new( "Behavior:" );//Describe what is actually happening.
    gtk_box_pack_start( GTK_BOX (box[0]), box[1] , TRUE, TRUE, BORDER_WID_TEXTF);
    gtk_container_add( GTK_CONTAINER (box[1]), fl.field[2]);
    gtk_widget_show( box[1] );

    box[1] = gtk_frame_new( "Reproduce:" );//Provide instructions for reproducing the issue.
    gtk_box_pack_start( GTK_BOX (box[0]), box[1] , TRUE, TRUE, BORDER_WID_TEXTF);
    gtk_container_add( GTK_CONTAINER (box[1]), fl.field[3]);
    gtk_widget_show( box[1] );

    box[1] = gtk_frame_new( "Notes:" );//This is a workspace. Eg: Who is assigned to work on this problem
    gtk_box_pack_start( GTK_BOX (box[0]), box[1] , TRUE, TRUE, BORDER_WID_TEXTF);
    gtk_container_add( GTK_CONTAINER (box[1]), fl.field[4]);
    gtk_widget_show( box[1] );

    box[1] = gtk_hbox_new( FALSE, 0 );
    gtk_box_pack_start( GTK_BOX (box[0]), box[1] , FALSE, FALSE, BORDER_WID_TEXTF);
    button = gtk_button_new_with_label( "Submit" );
    gtk_signal_connect( GTK_OBJECT (button), "clicked", (GtkSignalFunc) submit_bug, NULL );
    gtk_signal_connect( GTK_OBJECT (button), "clicked", (GtkSignalFunc) close_window, GTK_OBJECT (pop_up) );
    gtk_box_pack_end( GTK_BOX (box[1]), button , FALSE, FALSE, BORDER_WID_TEXTF);
    gtk_widget_show( button );
    button = gtk_button_new_with_label( "Cancel" );
    gtk_signal_connect( GTK_OBJECT (button), "clicked", (GtkSignalFunc) close_window, GTK_OBJECT (pop_up) );
    gtk_box_pack_end( GTK_BOX (box[1]), button , FALSE, FALSE, BORDER_WID_TEXTF);
    gtk_widget_show( button );

    gtk_widget_show( box[1] );

    for( i = 0; i < DB_FIELD_QT; i++ ) gtk_widget_show( fl.field[i] );
    gtk_widget_show( box[0] );
    gtk_widget_show( pop_up );

}/* End add_bug Func */


/******************************************************************************/
/* Function:    modify_select_bug                                             */
/* Parameters:  gpointer b       -- Attached Button                           */
/*              gpointer data    -- The buglist view (with row selected)      */
/* WARNING: Postpones g_freeing Id                                            */
/* HELPER FUNCTION: mop -- Free's Id of selected bug which had to be retained */
/*                          to be passed to update_db() via Update button.    */
/******************************************************************************/
void mop( gpointer a, gchar* leak ) { g_free(leak); }/* Helper Function */
void modify_select_bug( gpointer b, gpointer data )
{
    extern FIELD_LIST fl;
    extern sqlite3* bugdb;
    extern GtkListStore* buglist;
    GtkTreeModel* Buglist = GTK_TREE_MODEL(buglist);/*Get Rid of Warning*/

    GtkTreeIter iter;

    GtkTreeSelection* selection = gtk_tree_view_get_selection( GTK_TREE_VIEW (data) );
    if( gtk_tree_selection_get_selected( selection, &Buglist, &iter ) ) {
        /* Local Variables */
        gchar* bug_name, * Id;
        sqlite3_stmt* ppStmt;

        gtk_tree_model_get( GTK_TREE_MODEL (buglist), &iter, NAME_COL, &bug_name, ID_COL, &Id, -1 );

        if( sqlite3_prepare_v2( bugdb, "SELECT Expectation, Behavior, Reproduce, Notes FROM bug_List WHERE Id = ?", -1, &ppStmt, NULL ) == SQLITE_OK ) {
            int errn = sqlite3_bind_text( ppStmt, 1, Id, -1, NULL );
            if( errn != SQLITE_OK ) {
                g_print( "\nERROR: binding sqlite stmt part failed with Error #%d\n", errn );
            } else {
                if( sqlite3_step(ppStmt) == SQLITE_ROW ) {
                    /* Local Variables */
                    GtkTextIter text_iter;
                    GtkWidget* dialog, * content_area, * box;
                    GtkTextBuffer * buff;
                    int i;

                    for( i = 0; i < DB_FIELD_QT; i++ ) fl.field[i] = gtk_text_view_new();

                    dialog =  gtk_dialog_new();
                    gtk_window_set_title( GTK_WINDOW (dialog), bug_name );
                    content_area = gtk_dialog_get_content_area( GTK_DIALOG (dialog) );
                    g_signal_connect_swapped( dialog, "response", G_CALLBACK (gtk_widget_destroy), dialog );
                    gtk_window_set_default_size( GTK_WINDOW (dialog), DEFAULT_POPUP_SIZE );

                    box = gtk_frame_new( "Expectation:" );
                    gtk_container_add( GTK_CONTAINER (content_area), box );
                    gtk_container_add( GTK_CONTAINER (box), fl.field[1] );
                    buff = gtk_text_view_get_buffer( GTK_TEXT_VIEW (fl.field[1]) );
                    gtk_text_buffer_get_start_iter( buff, &text_iter );
                    gtk_text_buffer_insert( buff, &text_iter, (gchar*)(sqlite3_column_text( ppStmt, 0 )), -1 );

                    box = gtk_frame_new( "Behavior:" );
                    gtk_container_add( GTK_CONTAINER (content_area), box );
                    gtk_container_add( GTK_CONTAINER (box), fl.field[2] );
                    buff = gtk_text_view_get_buffer( GTK_TEXT_VIEW (fl.field[2]) );
                    gtk_text_buffer_get_start_iter( buff, &text_iter );
                    gtk_text_buffer_insert( buff, &text_iter, (gchar*)(sqlite3_column_text( ppStmt, 1 )), -1 );

                    box = gtk_frame_new( "Reproduce:" );
                    gtk_container_add( GTK_CONTAINER (content_area), box );
                    gtk_container_add( GTK_CONTAINER (box), fl.field[3] );
                    buff = gtk_text_view_get_buffer( GTK_TEXT_VIEW (fl.field[3]) );
                    gtk_text_buffer_get_start_iter( buff, &text_iter );
                    gtk_text_buffer_insert( buff, &text_iter, (gchar*)(sqlite3_column_text( ppStmt, 2 )), -1 );

                    box = gtk_frame_new( "Notes:" );
                    gtk_container_add( GTK_CONTAINER (content_area), box );
                    gtk_container_add( GTK_CONTAINER (box), fl.field[4] );
                    buff = gtk_text_view_get_buffer( GTK_TEXT_VIEW (fl.field[4]) );
                    gtk_text_buffer_get_start_iter( buff, &text_iter );
                    gtk_text_buffer_insert( buff, &text_iter, (gchar*)(sqlite3_column_text( ppStmt, 3 )), -1 );

                    content_area = gtk_dialog_get_action_area( GTK_DIALOG (dialog) );/* REUSING content_area */

                    box = gtk_button_new_with_label( "Cancel" ); /* REUSING - Not a Box */
                    gtk_signal_connect( GTK_OBJECT (box), "clicked", (GtkSignalFunc) close_window, GTK_OBJECT (dialog) );
                    gtk_signal_connect( GTK_OBJECT (box), "clicked", (GtkSignalFunc) mop, Id );
                    gtk_box_pack_end( GTK_BOX (content_area), box , FALSE, FALSE, BORDER_WID_TEXTF);

                    box = gtk_button_new_with_label( "Update" ); /* REUSING - Not a Box */
                    gtk_signal_connect( GTK_OBJECT (box), "clicked", (GtkSignalFunc) update_db, Id );
                    gtk_signal_connect( GTK_OBJECT (box), "clicked", (GtkSignalFunc) close_window, GTK_OBJECT (dialog) );
                    gtk_box_pack_end( GTK_BOX (content_area), box , FALSE, FALSE, BORDER_WID_TEXTF);

                    gtk_widget_show_all( dialog );
                } else g_print( "\nERROR: Bug ID not found in database!\n" );
            }/* End ppStmt bind ok Else */
            sqlite3_finalize(ppStmt);/* Clean Up */
        } else g_print("\nERROR: preparing sqlite stmt\n");
        g_free( bug_name ); //g_free( Id );/* Clean Up --Partially Postponed */
    } else g_print( "no row selected.\n" );
}/* End modify_select_bug Func */


/******************************************************************************/
/* Function:    open_reproduce_window                                         */
/* Parameters:  gpointer    b       -- Attached Button                        */
/*              gpointer    data    -- The buglist view (with row selected)   */
/******************************************************************************/
void open_reproduce_window( gpointer b, gpointer data )
{
    extern sqlite3* bugdb;
    extern GtkListStore* buglist;
    GtkTreeModel* Buglist = GTK_TREE_MODEL(buglist);/*Get Rid of Warning*/

    GtkTreeIter iter;

    GtkTreeSelection* selection = gtk_tree_view_get_selection( GTK_TREE_VIEW (data) );
    if( gtk_tree_selection_get_selected( selection, &Buglist, &iter ) ) {
        /* Local Variables */
        gchar* bug_name, * Id;
        sqlite3_stmt* ppStmt;

        gtk_tree_model_get( GTK_TREE_MODEL (buglist), &iter, NAME_COL, &bug_name, ID_COL, &Id, -1 );

        if( sqlite3_prepare_v2( bugdb, "SELECT Reproduce FROM bug_List WHERE Id = ?", -1, &ppStmt, NULL ) == SQLITE_OK ) {
            int errn = sqlite3_bind_text( ppStmt, 1, Id, -1, NULL );
            if( errn != SQLITE_OK ) {
                g_print( "\nERROR: binding sqlite stmt part failed with Error #%d\n", errn );
            } else {
                if( sqlite3_step(ppStmt) == SQLITE_ROW ) {
                    /* Local Variables */
                    GtkTextIter text_iter;
                    GtkWidget* field, * dialog, * content_area, * box;
                    GtkTextBuffer * buff;

                    field = gtk_text_view_new();
                    gtk_text_view_set_editable( GTK_TEXT_VIEW (field), FALSE );
                    gtk_text_view_set_cursor_visible( GTK_TEXT_VIEW (field), FALSE );

                    dialog = gtk_dialog_new_with_buttons( bug_name, NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_STOCK_CLOSE, NULL );
                    content_area = gtk_dialog_get_content_area( GTK_DIALOG (dialog) );
                    g_signal_connect_swapped( dialog, "response", G_CALLBACK (gtk_widget_destroy), dialog );
                    gtk_window_set_default_size( GTK_WINDOW (dialog), DEFAULT_POPUP_SIZE );

                    box = gtk_frame_new( "Reproduce:" );
                    gtk_container_add( GTK_CONTAINER (content_area), box );
                    gtk_container_add( GTK_CONTAINER (box), field );
                    buff = gtk_text_view_get_buffer( GTK_TEXT_VIEW (field) );
                    gtk_text_buffer_get_start_iter( buff, &text_iter );
                    gtk_text_buffer_insert( buff, &text_iter, (gchar*)(sqlite3_column_text( ppStmt, 0 )), -1 );

                    gtk_widget_show_all( dialog );
                } else g_print( "\nERROR: Bug ID not found in database!\n" );
            }/* End ppStmt bind ok Else */
            sqlite3_finalize(ppStmt);/* Clean Up */
        } else g_print("\nERROR: preparing sqlite stmt\n");
        g_free( bug_name ); g_free( Id );/* Clean Up */
    } else g_print( "no row selected.\n" );
}/* End open_reproduce_window Func */


/******************************************************************************/
/* Function:    open_behave_window                                            */
/* Parameters:  gpointer    b       -- Attached Button                        */
/*              gpointer    data    -- The buglist view (with row selected)   */
/******************************************************************************/
void open_behave_window( gpointer b, gpointer data )
{
    extern sqlite3* bugdb;
    extern GtkListStore* buglist;
    GtkTreeModel* Buglist = GTK_TREE_MODEL(buglist);/*Get Rid of Warning*/

    GtkTreeIter iter;

    GtkTreeSelection* selection = gtk_tree_view_get_selection( GTK_TREE_VIEW (data) );
    if( gtk_tree_selection_get_selected( selection, &Buglist, &iter ) ) {
        /* Local Variables */
        gchar* bug_name, * Id;
        sqlite3_stmt* ppStmt;

        gtk_tree_model_get( GTK_TREE_MODEL (buglist), &iter, NAME_COL, &bug_name, ID_COL, &Id, -1 );

        if( sqlite3_prepare_v2( bugdb, "SELECT Expectation, Behavior, Notes FROM bug_List WHERE Id = ?", -1, &ppStmt, NULL ) == SQLITE_OK ) {
            int errn = sqlite3_bind_text( ppStmt, 1, Id, -1, NULL );
            if( errn != SQLITE_OK ) {
                g_print( "\nERROR: binding sqlite stmt part failed with Error #%d\n", errn );
            } else {
                if( sqlite3_step(ppStmt) == SQLITE_ROW ) {
                    /* Local Variables */
                    GtkTextIter text_iter;
                    GtkWidget* field[3], * dialog, * content_area, * box;
                    GtkTextBuffer * buff;
                    int i;

                    for( i = 0; i < 3; i++ ) {
                        field[i] = gtk_text_view_new();
                        gtk_text_view_set_editable( GTK_TEXT_VIEW (field[i]), FALSE );
                        gtk_text_view_set_cursor_visible( GTK_TEXT_VIEW (field[i]), FALSE );
                    }

                    dialog = gtk_dialog_new_with_buttons( bug_name, NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_STOCK_CLOSE, NULL );
                    content_area = gtk_dialog_get_content_area( GTK_DIALOG (dialog) );
                    g_signal_connect_swapped( dialog, "response", G_CALLBACK (gtk_widget_destroy), dialog );
                    gtk_window_set_default_size( GTK_WINDOW (dialog), DEFAULT_POPUP_SIZE );

                    box = gtk_frame_new( "Expectation:" );
                    gtk_container_add( GTK_CONTAINER (content_area), box );
                    gtk_container_add( GTK_CONTAINER (box), field[0] );
                    buff = gtk_text_view_get_buffer( GTK_TEXT_VIEW (field[0]) );
                    gtk_text_buffer_get_start_iter( buff, &text_iter );
                    gtk_text_buffer_insert( buff, &text_iter, (gchar*)(sqlite3_column_text( ppStmt, 0 )), -1 );

                    box = gtk_frame_new( "Behavior:" );
                    gtk_container_add( GTK_CONTAINER (content_area), box );
                    gtk_container_add( GTK_CONTAINER (box), field[1] );
                    buff = gtk_text_view_get_buffer( GTK_TEXT_VIEW (field[1]) );
                    gtk_text_buffer_get_start_iter( buff, &text_iter );
                    gtk_text_buffer_insert( buff, &text_iter, (gchar*)(sqlite3_column_text( ppStmt, 1 )), -1 );

                    box = gtk_frame_new( "Notes:" );
                    gtk_container_add( GTK_CONTAINER (content_area), box );
                    gtk_container_add( GTK_CONTAINER (box), field[2] );
                    buff = gtk_text_view_get_buffer( GTK_TEXT_VIEW (field[2]) );
                    gtk_text_buffer_get_start_iter( buff, &text_iter );
                    gtk_text_buffer_insert( buff, &text_iter, (gchar*)(sqlite3_column_text( ppStmt, 2 )), -1 );

                    gtk_widget_show_all( dialog );
                } else g_print( "\nERROR: Bug ID not found in database!\n" );
            }/* End ppStmt bind ok Else */
            sqlite3_finalize(ppStmt);/* Clean Up */
        } else g_print("\nERROR: preparing sqlite stmt\n");
        g_free( bug_name ); g_free( Id );/* Clean Up */
    } else g_print( "no row selected.\n" );
}/* End open_behave_window Func */


/******************************************************************* HELPERS: */
/******************************************************************************/
/* Function:    load_pixbuf                                                   */
/* Parameters:  const gchar ptr     filename    -- Immage to load             */
/* Returns:     GdkPixbuf ptr                   -- The loaded immage          */
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

