/* $Id: datab.c,v 1.1 2013/09/19 03:48:12 moonsdad Exp $ */
#include "bugd.h"

/* Global Variables */
extern sqlite3* bugdb;
extern gboolean opendb;



void event_select( GtkWidget* clist, gint row, gint col, GdkEventButton* event, gpointer data )
{
    gchar* text;

    gtk_clist_get_text(GTK_CLIST(clist), row, 0, &text);

    g_print( "\nSelected Key ID# == %s\n", text );

    return;
}/* End event_select Func */

void change_status( gpointer data )
{

    return;
}/* End change_status Func */
