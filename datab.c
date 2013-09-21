/* $Id: datab.c,v 1.11 2013/09/21 06:12:48 moonsdad Exp $ */
#include "bugd.h"

//TODO: use sqlite_freemem() where necessary            and:
//    char* errmsg;//    if( errmsg ) fprintf( stderr, "\nERROR:%s\n", errmsg );

/******************************************************************************/
/* Function:   event_select                                                   */
/* Parameters: */
/* WARNING: */
/******************************************************************************/
void event_select( GtkWidget* clist, gint row, gint col, GdkEventButton* event, gpointer data )
{
    gchar* text;
    extern int select_keyval;

    gtk_clist_get_text(GTK_CLIST(clist), row, 0, &text);

    select_keyval = atoi(text);

    g_print( "\nSelected Key ID# == %s == %d\n", text, select_keyval );

    return;
}/* End event_select Func */


/******************************************************************************/
/* Function:   submit_bug                                                   */
/* Parameters: */
/* WARNING: */
/******************************************************************************/
void submit_bug( void )
{
    extern FIELD_LIST fl;
    extern sqlite3* bugdb;
    extern GtkListStore* buglist;

    GtkTextBuffer* buffer[DB_FIELD_QT];
    GtkTextIter start_iter[DB_FIELD_QT];
    GtkTextIter end_iter[DB_FIELD_QT];

    GtkTreeIter iter;

    sqlite3_stmt* ppStmt;
    int i, errn;

    for( i = 0; i < DB_FIELD_QT; i++ ) {
        buffer[i] = gtk_text_view_get_buffer( GTK_TEXT_VIEW (fl.field[i]) );
        if( buffer[i] ) {
            g_print( "\nGot buffer %d from field\n", i );
            gtk_text_buffer_get_start_iter( buffer[i], &start_iter[i] );
            gtk_text_buffer_get_end_iter( buffer[i], &end_iter[i] );
            g_print("\n%s\n", gtk_text_buffer_get_text( buffer[i], &start_iter[i], &end_iter[i], FALSE ));
        } else {
            g_print( "\nERROR: Failed to get buffer %d from field\n", i );
        }
    }

    if( sqlite3_prepare_v2( bugdb, "INSERT INTO bug_list( Name, Reproduce, Expectation, Behavior, Notes, Status ) VALUES(?,?,?,?,?,0)", -1, &ppStmt, NULL ) == SQLITE_OK ) {
        for( i = 0; i < DB_FIELD_QT; i++ ) {
            errn = sqlite3_bind_text( ppStmt,(i+1), gtk_text_buffer_get_text( buffer[i], &start_iter[i], &end_iter[i], FALSE ), -1, NULL );
            if(  errn != SQLITE_OK ) {
                g_print( "\nERROR: binding sqlite stmt part ?%d failed with Error #%d\n" , i+1, errn );
                sqlite3_finalize(ppStmt);
                return;
            }
        }
        sqlite3_step(ppStmt);
        sqlite3_finalize(ppStmt);
    } else g_print("\nERROR: preparing sqlite stmt\n");

    if( sqlite3_prepare_v2( bugdb, "SELECT Id, Status, Name FROM bug_List WHERE Name = ?", -1, &ppStmt, NULL ) == SQLITE_OK ) {
        errn = sqlite3_bind_text( ppStmt, 1, gtk_text_buffer_get_text( buffer[0], &start_iter[0], &end_iter[0], FALSE ), -1, NULL );
        if(  errn != SQLITE_OK ) {
            g_print( "\nERROR: binding sqlite stmt part failed with Error #%d\n", errn );
            sqlite3_finalize(ppStmt);
            return;
        }
        if( sqlite3_step(ppStmt) == SQLITE_ROW ) {
            gtk_list_store_append( buglist, &iter );
            gtk_list_store_set( buglist, &iter, ID_COL, sqlite3_column_text( ppStmt, 0 ), STATUS_COL, sqlite3_column_text( ppStmt, 1 ), NAME_COL, sqlite3_column_text( ppStmt, 2 ), -1 );
        } else g_print("\nERROR: Reload your database. (close and then open)\n");
        sqlite3_finalize(ppStmt);
    } else g_print("\nERROR: preparing sqlite stmt\n");

}


/******************************************************************************/
/* Function:   change_status                                                  */
/* Parameters: gpointer data*/
/* WARNING: */
/******************************************************************************/
void change_status( gpointer data )
{

    return;
}/* End change_status Func */

/******************************************************************************/
/* Function:   load_open_datab                                                */
/* Parameters: void* pArg, int argc, char** argv, char** columnNames */
/* Returns:     int    -   SQLite API expects this to be 0, Aborts otherwise. */
/* WARNING: */
/******************************************************************************/
int load_open_datab( void* pArg, int argc, char** argv, char** columnNames )
{
    extern GtkListStore* buglist;
    //gchar* abug[] = { argv[0], argv[6], argv[1] };//TODO: ReFormat Query
    GtkTreeIter iter;

    int i;
    for( i = 0; i < argc; i++ ) {
        printf("%s = %s\n", columnNames[i], argv[i] ? argv[i] : "NULL");
    }

    //gtk_clist_append( GTK_CLIST (buglist), abug );

    gtk_list_store_append( buglist, &iter );
    gtk_list_store_set( buglist, &iter, ID_COL, argv[0], STATUS_COL, argv[6], NAME_COL, argv[1], -1 );
    return 0;
}/* End load_open_datab Func */


/******************************************************************************/
/******************************************************************************/
int load_list_datab( void* pArg, int argc, char** argv, char** columnNames )
{
    extern GtkListStore* buglist;
    GtkTreeIter iter;

    gtk_list_store_append( buglist, &iter );
    gtk_list_store_set( buglist, &iter, ID_COL, argv[0], STATUS_COL, argv[1], NAME_COL, argv[2], -1 );
    return 0;
}/* End load_list_datab Func */


/******************************************************************************/
/* Function:    */
/* Parameters: void* pArg, int argc, char** argv, char** columnNames */
/* Returns:     int    -   SQLite API expects this to be 0, Aborts otherwise. */
/* WARNING: */
/******************************************************************************/
// int Callback( void* pArg, int argc, char** argv, char** columnNames )
// {
//   return 0;
// }/* End Func */
