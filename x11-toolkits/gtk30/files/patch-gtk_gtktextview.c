--- gtk/gtktextview.c.orig	2024-01-24 01:14:34 UTC
+++ gtk/gtktextview.c
@@ -9563,7 +9563,8 @@ popup_targets_received (GtkClipboard     *clipboard,
           g_signal_connect_swapped (menuitem, "activate",
                                     G_CALLBACK (gtk_text_view_insert_emoji), text_view);
           gtk_widget_show (menuitem);
-          gtk_menu_shell_append (GTK_MENU_SHELL (priv->popup_menu), menuitem);
+          if (g_getenv ("GTKM_INSERT_EMOJI") != FALSE)
+            gtk_menu_shell_append (GTK_MENU_SHELL (priv->popup_menu), menuitem);
         }
 
       g_signal_emit (text_view, signals[POPULATE_POPUP],
