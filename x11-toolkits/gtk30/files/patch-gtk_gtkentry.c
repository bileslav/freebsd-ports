--- gtk/gtkentry.c.orig	2024-01-24 01:14:34 UTC
+++ gtk/gtkentry.c
@@ -251,6 +251,9 @@ struct _GtkEntryPrivate
 
   gint64        handle_place_time;
 
+  gint          last_selection_start;
+  gint          last_selection_end;
+
   guint         shadow_type             : 4;
   guint         editable                : 1;
   guint         show_emoji_icon         : 1;
@@ -7404,11 +7407,12 @@ primary_get_cb (GtkClipboard     *clipboard,
 		gpointer          data)
 {
   GtkEntry *entry = GTK_ENTRY (data);
-  gint start, end;
-  
-  if (gtk_editable_get_selection_bounds (GTK_EDITABLE (entry), &start, &end))
+  GtkEntryPrivate *priv = entry->priv;
+  if (priv->last_selection_start != priv->last_selection_end)
     {
-      gchar *str = _gtk_entry_get_display_text (entry, start, end);
+      gchar *str = _gtk_entry_get_display_text (entry,
+                                                priv->last_selection_start,
+                                                priv->last_selection_end);
       gtk_selection_data_set_text (selection_data, str, -1);
       g_free (str);
     }
@@ -7421,12 +7425,16 @@ primary_clear_cb (GtkClipboard *clipboard,
   GtkEntry *entry = GTK_ENTRY (data);
   GtkEntryPrivate *priv = entry->priv;
 
+  priv->last_selection_start = 0;
+  priv->last_selection_end = 0;
+
   gtk_editable_select_region (GTK_EDITABLE (entry), priv->current_pos, priv->current_pos);
 }
 
 static void
 gtk_entry_update_primary_selection (GtkEntry *entry)
 {
+  GtkEntryPrivate *priv = entry->priv;
   GtkTargetList *list;
   GtkTargetEntry *targets;
   GtkClipboard *clipboard;
@@ -7445,14 +7453,12 @@ gtk_entry_update_primary_selection (GtkEntry *entry)
   
   if (gtk_editable_get_selection_bounds (GTK_EDITABLE (entry), &start, &end))
     {
+      priv->last_selection_start = start;
+      priv->last_selection_end = end;
+
       gtk_clipboard_set_with_owner (clipboard, targets, n_targets,
                                     primary_get_cb, primary_clear_cb, G_OBJECT (entry));
     }
-  else
-    {
-      if (gtk_clipboard_get_owner (clipboard) == G_OBJECT (entry))
-	gtk_clipboard_clear (clipboard);
-    }
 
   gtk_target_table_free (targets, n_targets);
   gtk_target_list_unref (list);
@@ -9632,7 +9638,8 @@ popup_targets_received (GtkClipboard     *clipboard,
           g_signal_connect_swapped (menuitem, "activate",
                                     G_CALLBACK (gtk_entry_insert_emoji), entry);
           gtk_widget_show (menuitem);
-          gtk_menu_shell_append (GTK_MENU_SHELL (menu), menuitem);
+          if (g_getenv ("GTKM_INSERT_EMOJI") != FALSE)
+            gtk_menu_shell_append (GTK_MENU_SHELL (menu), menuitem);
         }
 
       g_signal_emit (entry, signals[POPULATE_POPUP], 0, menu);
