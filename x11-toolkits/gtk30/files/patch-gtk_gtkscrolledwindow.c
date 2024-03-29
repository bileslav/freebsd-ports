--- gtk/gtkscrolledwindow.c.orig	2024-01-24 01:14:34 UTC
+++ gtk/gtkscrolledwindow.c
@@ -4548,7 +4548,7 @@ gtk_scrolled_window_update_use_indicators (GtkScrolled
 
   use_indicators = overlay_scrolling && priv->overlay_scrolling;
 
-  if (g_strcmp0 (g_getenv ("GTK_OVERLAY_SCROLLING"), "0") == 0)
+  if (g_strcmp0 (g_getenv ("GTK_OVERLAY_SCROLLING"), "1") != 0)
     use_indicators = FALSE;
 
   if (priv->use_indicators != use_indicators)
