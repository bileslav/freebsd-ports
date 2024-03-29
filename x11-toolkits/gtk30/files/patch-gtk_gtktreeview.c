--- gtk/gtktreeview.c.orig	2024-01-24 01:14:34 UTC
+++ gtk/gtktreeview.c
@@ -5303,6 +5303,18 @@ gtk_tree_view_bin_draw (GtkWidget      *widget,
 
           gtk_style_context_add_class (context, GTK_STYLE_CLASS_CELL);
 
+    if (list == first_column)
+      gtk_style_context_add_class (context, rtl ? "last" : "first");
+    if (list == last_column)
+      gtk_style_context_add_class (context, rtl ? "first" : "last"); 
+    if (_gtk_rbtree_node_get_index (tree, node) % 2 == 0)
+      gtk_style_context_add_class (context, "odd");
+    else
+      gtk_style_context_add_class (context, "even");
+
+    if (flags & GTK_CELL_RENDERER_SORTED)
+      gtk_style_context_add_class (context, "sorted");
+
 	  if (node == tree_view->priv->cursor_node && has_can_focus_cell
               && ((column == tree_view->priv->focus_column
                    && tree_view->priv->draw_keyfocus &&
