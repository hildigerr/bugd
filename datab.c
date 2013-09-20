/* $Id: datab.c,v 1.4 2013/09/20 02:26:10 moonsdad Exp $ */
#include "bugd.h"


/******************************************************************************/
/* Function:   event_select                                                   */
/* Parameters: */
/* WARNING: */
/******************************************************************************/
void event_select( GtkWidget* clist, gint row, gint col, GdkEventButton* event, gpointer data )
{
    gchar* text;

    gtk_clist_get_text(GTK_CLIST(clist), row, 0, &text);

    g_print( "\nSelected Key ID# == %s\n", text );

    return;
}/* End event_select Func */


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
   int i;
   for( i = 0; i < argc; i++ ) {
      printf("%s = %s\n", columnNames[i], argv[i] ? argv[i] : "NULL");
   }

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
