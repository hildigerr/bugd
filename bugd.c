/* $Id: bugd.c,v 1.3 2013/09/17 21:15:06 moonsdad Exp $ */

/* bugd - A simple Bug Database interface using SQLite and GTK */
#include <stdlib.h>
#include <gtk/gtk.h>
#include <sqlite3.h>

/* ARRAY ACCESS CONSTANTS */
#define H_BOX 0
#define OUTER 0
#define INNER 1

/* PREFERENCES */
#define DEFAULT_WINDOW_SIZE 273,373
#define BORDER_WID_OUTER 10
#define BORDER_WID_INNER 5
#define BUG_LIST_COLS 3

/******************************************************************CALLBACKS: */
void add_bug( gpointer data )
{
//     int status = 0;
//     int next_keyval;
//     gchar* nubug[BUG_LIST_COLS] = { "0", "0", "A Bug" };
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
//     gtk_clist_append( GTK_CLIST (data), nubug );

    return;
}
void change_status( gpointer data )
{

    return;
}
void change_display_list( gpointer data )
{

    return;
}
void open_reproduce_window( gpointer data )
{

    return;
}
void open_behave_window( gpointer data )
{

    return;
}
/* If we come here, then the user has selected a row in the list. *///TODO
void event_select( GtkWidget* clist, gint row, gint column, GdkEventButton* event, gpointer data )
{
    gchar *text;

    /* Get the text that is stored in the selected row and column
     * which was clicked in. We will receive it as a pointer in the
     * argument text.
     */
    gtk_clist_get_text(GTK_CLIST(clist), row, column, &text);

    /* Just prints some information about the selected row */
    g_print("You selected row %d. More specifically you clicked in "
            "column %d, and the text in this cell is %s\n\n",
            row, column, text);

    return;
}

/********************************************************************** MAIN: */
int main( int argc, char **argv )
{
    GtkWidget* window[2];
    GtkWidget* table[2];
    GtkWidget* buglist;
    GtkWidget* button;
    gchar* buglist_col_titles[BUG_LIST_COLS] = { "ID#", "STATUS", "NAME" };

    sqlite3* bugdb;

    if( sqlite3_open( "test.db", &bugdb ) ) {
        fprintf( stderr, "Can't open database: %s\n", sqlite3_errmsg(bugdb));
        exit(1);
    }//TODO: Move to Menu->Open

    gtk_init( &argc, &argv );

    /* Setup Main Window */
    window[OUTER] = gtk_window_new( GTK_WINDOW_TOPLEVEL );
    gtk_window_set_title( GTK_WINDOW (window[OUTER]), "bugd" );
    g_signal_connect( window[OUTER], "destroy", G_CALLBACK (gtk_main_quit), NULL);
    gtk_container_set_border_width( GTK_CONTAINER (window[OUTER]), BORDER_WID_OUTER );
    gtk_window_set_default_size( GTK_WINDOW (window[OUTER]), DEFAULT_WINDOW_SIZE );
    table[OUTER] = gtk_vbox_new (FALSE, 0);
    gtk_container_add( GTK_CONTAINER (window[OUTER]), table[OUTER] );

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
    gtk_box_pack_start( GTK_BOX (table[OUTER]), table[INNER], FALSE, TRUE, 5 );
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

    sqlite3_close(bugdb);
    return 0;
}/* End main Func */
