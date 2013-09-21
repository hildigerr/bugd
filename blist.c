#include "bugd.h"

GtkWidget* init_list( void )
{
    extern GtkListStore* buglist;

//    gchar* buglist_col_titles[BUG_LIST_COLS] = { "ID#", "STATUS", "NAME" };
    GtkCellRenderer* renderer;
    GtkTreeViewColumn* column;
    GtkWidget* view;

    view = gtk_tree_view_new_with_model( GTK_TREE_MODEL (buglist) );

    renderer = gtk_cell_renderer_text_new();
    g_object_set( G_OBJECT (renderer), "editable", FALSE, NULL );
    column = gtk_tree_view_column_new_with_attributes( "ID#", renderer, "text", ID_COL, NULL );
    gtk_tree_view_append_column(GTK_TREE_VIEW (view), column);

    renderer = gtk_cell_renderer_text_new();
    g_object_set( G_OBJECT (renderer), "editable", FALSE, NULL );
    column = gtk_tree_view_column_new_with_attributes( "STATUS", renderer, "text", STATUS_COL, NULL );
    gtk_tree_view_append_column( GTK_TREE_VIEW (view), column );

    renderer = gtk_cell_renderer_text_new();
    g_object_set( G_OBJECT (renderer), "editable", FALSE, NULL );
    column = gtk_tree_view_column_new_with_attributes ("NAME", renderer, "text", NAME_COL, NULL );
    gtk_tree_view_append_column( GTK_TREE_VIEW (view), column );

    gtk_tree_view_set_headers_visible( GTK_TREE_VIEW (view), TRUE );
    gtk_widget_show( view );
    return view;
}
