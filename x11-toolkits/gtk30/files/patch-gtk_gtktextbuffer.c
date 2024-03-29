--- gtk/gtktextbuffer.c.orig	2024-01-24 01:14:34 UTC
+++ gtk/gtktextbuffer.c
@@ -72,6 +72,9 @@ struct _GtkTextBufferPrivate
   GSList *clipboard_contents_buffers;
   GSList *selection_clipboards;
 
+  GtkTextMark *last_selection_start;
+  GtkTextMark *last_selection_end;
+
   GtkTextLogAttrCache *log_attr_cache;
 
   guint user_action_count;
@@ -804,6 +807,18 @@ gtk_text_buffer_finalize (GObject *object)
   buffer = GTK_TEXT_BUFFER (object);
   priv = buffer->priv;
 
+  if (priv->last_selection_start != NULL)
+    {
+      gtk_text_buffer_delete_mark (buffer, priv->last_selection_start);
+      priv->last_selection_start = NULL;
+    }
+
+  if (priv->last_selection_end != NULL)
+    {
+      gtk_text_buffer_delete_mark (buffer, priv->last_selection_end);
+      priv->last_selection_end = NULL;
+    }
+
   remove_all_selection_clipboards (buffer);
 
   if (priv->tag_table)
@@ -3189,11 +3204,24 @@ clipboard_clear_selection_cb (GtkClipboard *clipboard,
 clipboard_clear_selection_cb (GtkClipboard *clipboard,
                               gpointer      data)
 {
-  /* Move selection_bound to the insertion point */
   GtkTextIter insert;
   GtkTextIter selection_bound;
   GtkTextBuffer *buffer = GTK_TEXT_BUFFER (data);
+  GtkTextBufferPrivate *priv = buffer->priv;
 
+  if (priv->last_selection_start != NULL)
+    {
+      gtk_text_buffer_delete_mark (buffer, priv->last_selection_start);
+      priv->last_selection_start = NULL;
+    }
+
+  if (priv->last_selection_end != NULL)
+    {
+      gtk_text_buffer_delete_mark (buffer, priv->last_selection_end);
+      priv->last_selection_end = NULL;
+    }
+
+  /* Move selection_bound to the insertion point */
   gtk_text_buffer_get_iter_at_mark (buffer, &insert,
                                     gtk_text_buffer_get_insert (buffer));
   gtk_text_buffer_get_iter_at_mark (buffer, &selection_bound,
@@ -3215,9 +3243,16 @@ clipboard_get_selection_cb (GtkClipboard     *clipboar
                             gpointer          data)
 {
   GtkTextBuffer *buffer = GTK_TEXT_BUFFER (data);
+  GtkTextBufferPrivate *priv = buffer->priv;
   GtkTextIter start, end;
 
-  if (gtk_text_buffer_get_selection_bounds (buffer, &start, &end))
+  if (priv->last_selection_start == NULL || priv->last_selection_end == NULL)
+    return;
+
+  gtk_text_buffer_get_iter_at_mark (buffer, &start, priv->last_selection_start);
+  gtk_text_buffer_get_iter_at_mark (buffer, &end, priv->last_selection_end);
+
+  if (!gtk_text_iter_equal (&start, &end))
     {
       if (info == GTK_TEXT_BUFFER_TARGET_INFO_BUFFER_CONTENTS)
         {
@@ -3648,7 +3683,15 @@ clipboard_clipboard_buffer_received (GtkClipboard     
 	}
       else
 	{
-	  if (gtk_text_buffer_get_selection_bounds (src_buffer, &start, &end))
+          GtkTextBufferPrivate *priv = src_buffer->priv;
+
+          if (priv->last_selection_start == NULL || priv->last_selection_end == NULL)
+            return;
+
+          gtk_text_buffer_get_iter_at_mark (src_buffer, &start, priv->last_selection_start);
+          gtk_text_buffer_get_iter_at_mark (src_buffer, &end, priv->last_selection_end);
+
+          if (!gtk_text_iter_equal (&start, &end))
 	    paste_from_buffer (clipboard, request_data, src_buffer,
 			       &start, &end);
 	}
@@ -3683,39 +3726,41 @@ update_selection_clipboards (GtkTextBuffer *buffer)
 static void
 update_selection_clipboards (GtkTextBuffer *buffer)
 {
-  GtkTextBufferPrivate *priv;
   gboolean has_selection;
   GtkTextIter start;
   GtkTextIter end;
-  GSList *tmp_list;
+  GtkTextBufferPrivate *priv = buffer->priv;
+  GSList *l;
 
-  priv = buffer->priv;
+  if (!gtk_text_buffer_get_selection_bounds (buffer, &start, &end))
+    return;
 
+  if (priv->last_selection_start != NULL)
+    gtk_text_buffer_move_mark (buffer, priv->last_selection_start, &start);
+  else
+    priv->last_selection_start = gtk_text_buffer_create_mark (buffer, NULL, &start, FALSE);
+
+  if (priv->last_selection_end != NULL)
+    gtk_text_buffer_move_mark (buffer, priv->last_selection_end, &end);
+  else
+    priv->last_selection_end = gtk_text_buffer_create_mark (buffer, NULL, &end, TRUE);
+
   gtk_text_buffer_get_copy_target_list (buffer);
   has_selection = gtk_text_buffer_get_selection_bounds (buffer, &start, &end);
-  tmp_list = buffer->priv->selection_clipboards;
 
-  while (tmp_list)
+  for (l = priv->selection_clipboards; l != NULL; l = l->next)
     {
-      SelectionClipboard *selection_clipboard = tmp_list->data;
+      SelectionClipboard *selection_clipboard = l->data;
       GtkClipboard *clipboard = selection_clipboard->clipboard;
 
-      if (has_selection)
-        {
-          /* Even if we already have the selection, we need to update our
-           * timestamp.
-           */
-          gtk_clipboard_set_with_owner (clipboard,
-                                        priv->copy_target_entries,
-                                        priv->n_copy_target_entries,
-                                        clipboard_get_selection_cb,
-                                        clipboard_clear_selection_cb,
-                                        G_OBJECT (buffer));
-        }
-      else if (gtk_clipboard_get_owner (clipboard) == G_OBJECT (buffer))
-        gtk_clipboard_clear (clipboard);
-
-      tmp_list = tmp_list->next;
+      /* Even if we already have the selection, we need to update our timestamp. */
+      if (!gtk_clipboard_set_with_owner (clipboard,
+                                         priv->copy_target_entries,
+                                         priv->n_copy_target_entries,
+                                         clipboard_get_selection_cb,
+                                         clipboard_clear_selection_cb,
+                                         G_OBJECT (buffer)))
+        clipboard_clear_selection_cb (clipboard, buffer);
     }
 }
 
