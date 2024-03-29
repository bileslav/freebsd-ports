--- gtk/gtklabel.c.orig	2024-01-24 01:14:34 UTC
+++ gtk/gtklabel.c
@@ -267,6 +267,9 @@ struct _GtkLabelPrivate
   gfloat   xalign;
   gfloat   yalign;
 
+  gint     last_selection_anchor;
+  gint     last_selection_end;
+
   guint    mnemonics_visible  : 1;
   guint    jtype              : 2;
   guint    wrap               : 1;
@@ -3686,6 +3689,14 @@ gtk_label_get_preferred_layout_size (GtkLabel *label,
   GtkLabelPrivate *priv = label->priv;
   PangoLayout *layout;
   gint char_pixels;
+  GtkWidget *toplevel = gtk_widget_get_toplevel (GTK_WIDGET (label));
+  if (GTK_IS_WINDOW (toplevel))
+    {
+      gint default_window_width;
+      gtk_window_get_default_size (GTK_WINDOW (toplevel), &default_window_width, NULL);
+      if (default_window_width == -1 && priv->max_width_chars == -1 && priv->ellipsize == PANGO_ELLIPSIZE_NONE)
+        priv->max_width_chars = 56;
+    }
 
   /* "width-chars" Hard-coded minimum width:
    *    - minimum size should be MAX (width-chars, strlen ("..."));
@@ -5672,9 +5683,13 @@ static void
 }
 
 static void
-gtk_label_set_selection_text (GtkLabel         *label,
-			      GtkSelectionData *selection_data)
+gtk_label_drag_data_get (GtkWidget        *widget,
+			 GdkDragContext   *context,
+			 GtkSelectionData *selection_data,
+			 guint             info,
+			 guint             time)
 {
+  GtkLabel *label = GTK_LABEL (widget);
   GtkLabelPrivate *priv = label->priv;
 
   if (priv->select_info &&
@@ -5705,22 +5720,40 @@ static void
 }
 
 static void
-gtk_label_drag_data_get (GtkWidget        *widget,
-			 GdkDragContext   *context,
-			 GtkSelectionData *selection_data,
-			 guint             info,
-			 guint             time)
-{
-  gtk_label_set_selection_text (GTK_LABEL (widget), selection_data);
-}
-
-static void
 get_text_callback (GtkClipboard     *clipboard,
                    GtkSelectionData *selection_data,
                    guint             info,
                    gpointer          user_data_or_owner)
 {
-  gtk_label_set_selection_text (GTK_LABEL (user_data_or_owner), selection_data);
+  GtkLabel *label;
+  GtkLabelPrivate *priv;
+
+  label = GTK_LABEL (user_data_or_owner);
+  priv = label->priv;
+
+  if ((priv->last_selection_anchor != priv->last_selection_end) &&
+      priv->text)
+    {
+      gint start, end;
+      gint len;
+
+      start = MIN (priv->last_selection_anchor,
+                   priv->last_selection_end);
+      end = MAX (priv->last_selection_anchor,
+                 priv->last_selection_end);
+
+      len = strlen (priv->text);
+
+      if (end > len)
+        end = len;
+
+      if (start > len)
+        start = len;
+
+      gtk_selection_data_set_text (selection_data,
+				   priv->text + start,
+				   end - start);
+    }
 }
 
 static void
@@ -5733,6 +5766,9 @@ clear_text_callback (GtkClipboard     *clipboard,
   label = GTK_LABEL (user_data_or_owner);
   priv = label->priv;
 
+  priv->last_selection_anchor = 0;
+  priv->last_selection_end = 0;
+
   if (priv->select_info)
     {
       priv->select_info->selection_anchor = priv->select_info->selection_end;
@@ -5836,6 +5872,9 @@ gtk_label_select_region_index (GtkLabel *label,
           GtkTargetEntry *targets;
           gint n_targets;
 
+          priv->last_selection_anchor = anchor_index;
+          priv->last_selection_end = end_index;
+
           list = gtk_target_list_new (NULL, 0);
           gtk_target_list_add_text_targets (list, 0);
           targets = gtk_target_table_new_from_list (list, &n_targets);
@@ -5864,10 +5903,6 @@ gtk_label_select_region_index (GtkLabel *label,
         }
       else
         {
-          if (clipboard &&
-              gtk_clipboard_get_owner (clipboard) == G_OBJECT (label))
-            gtk_clipboard_clear (clipboard);
-
           if (priv->select_info->selection_node)
             {
               gtk_css_node_set_parent (priv->select_info->selection_node, NULL);
