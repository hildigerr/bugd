/* $Id: datab.c,v 1.20 2013/09/22 05:15:10 moonsdad Exp $ */
#include "bugd.h"

/******************************************************************************/
/* Function:   submit_bug                                                     */
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
            #ifdef DEBUG
                g_print( "\nGot buffer %d from field\n", i );
            #endif
            gtk_text_buffer_get_start_iter( buffer[i], &start_iter[i] );
            gtk_text_buffer_get_end_iter( buffer[i], &end_iter[i] );
            #ifdef DEBUG
                g_print("\n%s\n", gtk_text_buffer_get_text( buffer[i], &start_iter[i], &end_iter[i], FALSE ));
            #endif
        } else {
            g_print( "\nERROR: Failed to get buffer %d from field\n", i );
        }/* End !get buffer Else */
    }/* End DB_FIELD_QT For */

    if( sqlite3_prepare_v2( bugdb, "INSERT INTO bug_list( Name, Expectation, Behavior, Reproduce, Notes, Status ) VALUES(?,?,?,?,?,0)", -1, &ppStmt, NULL ) == SQLITE_OK ) {
        for( i = 0; i < DB_FIELD_QT; i++ ) {
            errn = sqlite3_bind_text( ppStmt,(i+1), gtk_text_buffer_get_text( buffer[i], &start_iter[i], &end_iter[i], FALSE ), -1, NULL );
            if(  errn != SQLITE_OK ) {
                g_print( "\nERROR: binding sqlite stmt part ?%d failed with Error #%d\n" , i+1, errn );
                sqlite3_finalize(ppStmt);
                return;
            }/* End !OK If */
        }/* End DB_FIELD_QT For */
        sqlite3_step(ppStmt);
        sqlite3_finalize(ppStmt);
    } else g_print("\nERROR: preparing sqlite stmt\n");

    if( sqlite3_prepare_v2( bugdb, "SELECT Id, Status, Name FROM bug_List WHERE Name = ?", -1, &ppStmt, NULL ) == SQLITE_OK ) {
        errn = sqlite3_bind_text( ppStmt, 1, gtk_text_buffer_get_text( buffer[0], &start_iter[0], &end_iter[0], FALSE ), -1, NULL );
        if(  errn != SQLITE_OK ) {
            g_print( "\nERROR: binding sqlite stmt part failed with Error #%d\n", errn );
            sqlite3_finalize(ppStmt);
            return;
        }/* End !OK If */
        if( sqlite3_step(ppStmt) == SQLITE_ROW ) {
            gtk_list_store_append( buglist, &iter );
            gtk_list_store_set( buglist, &iter, ID_COL, sqlite3_column_text( ppStmt, 0 ), STATUS_COL, sqlite3_column_text( ppStmt, 1 ), NAME_COL, sqlite3_column_text( ppStmt, 2 ), -1 );
        } else g_print("\nERROR: Reload your database. (close and then open)\n");
        sqlite3_finalize(ppStmt);
    } else g_print("\nERROR: preparing sqlite stmt\n");
}/* End submit_bug Func */


/******************************************************************************/
/* Function:   change_status                                                  */
/* Parameters: gpointer data                                                  */
/* WARNING:     This will only work in single or browse selection mode!       */
/* TODO: Allow variable status values with user defined meanings              */
/******************************************************************************/
void change_status( gpointer b, gpointer data )
{
    extern GtkListStore* buglist;
    extern sqlite3* bugdb;
    GtkTreeIter iter;
    sqlite3_stmt* ppStmt; int errn;
    GtkTreeModel* Buglist = GTK_TREE_MODEL(buglist);/*Get Rid of Warning*/

    GtkTreeSelection* selection = gtk_tree_view_get_selection( GTK_TREE_VIEW (data) );
    if( gtk_tree_selection_get_selected( selection, &Buglist, &iter ) ) {
        gchar* Id, * status;
        gtk_tree_model_get( GTK_TREE_MODEL (buglist), &iter, ID_COL, &Id, STATUS_COL, &status, -1 );
        gtk_list_store_set( buglist, &iter, STATUS_COL,( status[0] == '0' )? "1":"0" , -1 );

        if( sqlite3_prepare_v2( bugdb, "UPDATE bug_list SET Status = ? WHERE Id = ?", -1, &ppStmt, NULL ) == SQLITE_OK ) {
            errn = sqlite3_bind_text( ppStmt, 1, ( status[0] == '0' )? "1":"0" , -1, NULL );
            errn = sqlite3_bind_text( ppStmt, 2, Id , -1, NULL );
            if(  errn != SQLITE_OK ) g_print( "\nERROR: binding sqlite stmt part failed with Error #%d\n", errn );
            else sqlite3_step(ppStmt);
            sqlite3_finalize(ppStmt);
        } else g_print("\nERROR: preparing sqlite stmt\n");

        g_free( Id ); g_free( status );
    } else g_print( "no row selected.\n" );

}/* End change_status Func */


/******************************************************************************/
/* Function:    update_db                                                     */
/* Parameters:  gpointer    b       -- Attached Button                        */
/*              gchar*      data    -- Id of db entry to update               */
/* WARNING:     g_frees data to prevent memory leak.                          */
/******************************************************************************/
void update_db( gpointer b, gchar* data )
{
    extern FIELD_LIST fl;
    extern sqlite3* bugdb;

    GtkTextBuffer* buffer[DB_FIELD_QT];
    GtkTextIter start_iter[DB_FIELD_QT];
    GtkTextIter end_iter[DB_FIELD_QT];

    sqlite3_stmt* ppStmt;
    int i, errn;

    for( i = 1; i < DB_FIELD_QT; i++ ) {
        buffer[i] = gtk_text_view_get_buffer( GTK_TEXT_VIEW (fl.field[i]) );
        if( buffer[i] ) {
            #ifdef DEBUG
                g_print( "\nGot buffer %d from field\n", i );
            #endif
            gtk_text_buffer_get_start_iter( buffer[i], &start_iter[i] );
            gtk_text_buffer_get_end_iter( buffer[i], &end_iter[i] );
            #ifdef DEBUG
                g_print("\n%s\n", gtk_text_buffer_get_text( buffer[i], &start_iter[i], &end_iter[i], FALSE ));
            #endif
        } else {
            g_print( "\nERROR: Failed to get buffer %d from field\n", i );
        }/* End !get buffer Else */
    }/* End DB_FIELD_QT For */

    if( sqlite3_prepare_v2( bugdb, "UPDATE bug_list SET Expectation = ?, Behavior = ?, Reproduce = ?, Notes = ? WHERE Id = ?", -1, &ppStmt, NULL ) == SQLITE_OK ) {
        for( i = 1; i < DB_FIELD_QT; i++ ) {
            errn = sqlite3_bind_text( ppStmt, i, gtk_text_buffer_get_text( buffer[i], &start_iter[i], &end_iter[i], FALSE ), -1, NULL );
            if(  errn != SQLITE_OK ) {
                g_print( "\nERROR: binding sqlite stmt part ?%d failed with Error #%d\n" , i+1, errn );
                sqlite3_finalize(ppStmt);
                g_free(data);
                return;
            }/* End !OK If */
        }/* End DB_FIELD_QT For */
        errn = sqlite3_bind_text( ppStmt, i, data, -1, NULL ); /* (i == DB_FIELD_QT) from for-loop , data is Id */
        if( errn == SQLITE_OK ) {
            if( sqlite3_step(ppStmt) != SQLITE_DONE ) g_print("\nERROR: step failed: %s\n", sqlite3_errmsg(bugdb) );
        } else g_print( "\nERROR: binding sqlite stmt part ?%d failed with Error #%d\n" , i, errn );
        sqlite3_finalize(ppStmt);
    } else g_print("\nERROR: preparing sqlite stmt\n");

    g_free(data);

}/* End update_db Func */


/******************************************************************************/
/* Function:    load_open_datab                                               */
/* Parameters:  void* pArg                                                    */
/*              int argc        -- Qt of elements in argv                     */
/*              char** argv     -- Result Columns of sqlite3_exec             */
/*              char** columnNames                                            */
/* Returns:     int    --  SQLite API expects this to be 0, Aborts otherwise. */
/******************************************************************************/
int load_open_datab( void* pArg, int argc, char** argv, char** columnNames )
{
    extern GtkListStore* buglist;
    GtkTreeIter iter;

    #ifdef DEBUG
        int i;
        for( i = 0; i < argc; i++ )
            printf("%s = %s\n", columnNames[i], argv[i] ? argv[i] : "NULL");
    #endif

    gtk_list_store_append( buglist, &iter );
    gtk_list_store_set( buglist, &iter, ID_COL, argv[0], STATUS_COL, argv[1], NAME_COL, argv[2], -1 );
    return 0;
}/* End load_open_datab Func */



//TEMPLATE:
/******************************************************************************/
/* Function:    */
/* Parameters:  void* pArg, int argc, char** argv, char** columnNames */
/* Returns:     int    --  SQLite API expects this to be 0, Aborts otherwise. */
/* WARNING: */
/******************************************************************************/
// int Callback( void* pArg, int argc, char** argv, char** columnNames )
// {
//     extern GtkListStore* buglist;
//     GtkTreeIter iter;
//
//     gtk_list_store_append( buglist, &iter );
//     gtk_list_store_set( buglist, &iter, ID_COL, argv[0], STATUS_COL, argv[1], NAME_COL, argv[2], -1 );
//     return 0;
// }/* End Func */
