/* $Id: datab.c,v 1.3 2013/09/19 06:04:17 moonsdad Exp $ */
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
