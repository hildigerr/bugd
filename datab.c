/* $Id: datab.c,v 1.8 2013/09/20 21:27:23 moonsdad Exp $ */
#include "bugd.h"

//TODO: use sqlite_freemem() where necessary

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

    //g_print( "\nSelected Key ID# == %s", text );

    select_keyval = atoi(text);

    g_print( "\nSelected Key ID# == %d\n", select_keyval );

    return;
}/* End event_select Func */


/******************************************************************************/
/* Function:   submit_bug                                                   */
/* Parameters: */
/* WARNING: */
/******************************************************************************/
void submit_bug( )//FIELD_LIST* my )
{
    extern FIELD_LIST fl;
    extern sqlite3* bugdb;
//    extern GtkWidget* buglist;
//    char* errmsg;
    GtkTextBuffer* buffer[DB_FIELD_QT];

    GtkTextIter start_iter[DB_FIELD_QT];
    GtkTextIter end_iter[DB_FIELD_QT];

    sqlite3_stmt* ppStmt;

    int i, errn;

    for( i = 0; i < DB_FIELD_QT; i++ ) {
        buffer[i] = gtk_text_view_get_buffer( GTK_TEXT_VIEW (fl.field[i]) );
        if( buffer[i] ) {
            g_print( "\nGot buffer %d from field\n", i );
            gtk_text_buffer_get_start_iter( buffer[i], &start_iter[i] );
            gtk_text_buffer_get_end_iter( buffer[i], &end_iter[i] );
            //g_print("\n%s\n", gtk_text_buffer_get_text( buffer[i], &start_iter[i], &end_iter[i], FALSE ));
        } else {
            g_print( "\nERROR: Failed to get buffer %d from field\n", i );
        }
    }


    if( sqlite3_prepare_v2( bugdb, "INSERT INTO bug_list( Name, Reproduce, Expectation, Behavior, Notes, Status ) VALUES(?,?,?,?,?,0)", -1, &ppStmt, NULL ) == SQLITE_OK ) {
        for( i = 0; i < DB_FIELD_QT; i++ ) {
            errn = sqlite3_bind_text( ppStmt,(i+1), gtk_text_buffer_get_text( buffer[i], &start_iter[i], &end_iter[i], FALSE ), -1, NULL );
            if(  errn != SQLITE_OK ) {
                g_print( "\nERROR: binding sqlite stmt part ?%d failed with Error #%d\n" , i+1, errn );
                return;
            }
        }
        sqlite3_step(ppStmt);
        sqlite3_finalize(ppStmt);
//        sqlite3_reset(ppStmt);
    } else g_print("\nERROR: preparing sqlite stmt\n");



//     sqlite_exec_printf( bugdb, "INSERT INTO bug_list( Name, Reproduce, Expectation, Behavior, Notes, Status ) VALUES('%s','%s','%s','%s','%s',0)", NULL, NULL, &errmsg,
//         gtk_text_buffer_get_text( buffer[0], &start_iter[0], &end_iter[0], FALSE ),
//         gtk_text_buffer_get_text( buffer[1], &start_iter[1], &end_iter[1], FALSE ),
//         gtk_text_buffer_get_text( buffer[2], &start_iter[2], &end_iter[2], FALSE ),
//         gtk_text_buffer_get_text( buffer[3], &start_iter[3], &end_iter[3], FALSE ),
//         gtk_text_buffer_get_text( buffer[4], &start_iter[4], &end_iter[4], FALSE )
//     );

//    if( errmsg ) fprintf( stderr, "\nERROR:%s\n", errmsg );
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
    extern GtkWidget* buglist;
    gchar* abug[] = { argv[0], argv[6], argv[1] };

    int i;
    for( i = 0; i < argc; i++ ) {
        printf("%s = %s\n", columnNames[i], argv[i] ? argv[i] : "NULL");
    }

    gtk_clist_append( GTK_CLIST (buglist), abug );

    return 0;
}/* End load_open_datab Func */


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
