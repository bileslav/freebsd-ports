--- gtk/gtkwindow.c.orig	2024-01-24 01:14:34 UTC
+++ gtk/gtkwindow.c
@@ -148,7 +148,7 @@
  * widget that is added as a titlebar child.
  */
 
-#define MNEMONICS_DELAY 300 /* ms */
+#define MNEMONICS_DELAY 0 /* ms */
 #define NO_CONTENT_CHILD_NAT 200
 /* In case the content (excluding header bar and shadows) of the window
  * would be empty, either because there is no visible child widget or only an
@@ -4138,6 +4138,8 @@ gtk_window_supports_client_shadow (GtkWindow *window)
 #ifdef GDK_WINDOWING_X11
   if (GDK_IS_X11_DISPLAY (display))
     {
+      if (g_strcmp0 (g_getenv ("GTK_CSD"), "1") != 0)
+        return FALSE;
       if (!gdk_screen_is_composited (screen))
         return FALSE;
 
@@ -4189,6 +4191,14 @@ gtk_window_enable_csd (GtkWindow *window)
     }
 
   priv->client_decorated = TRUE;
+#ifdef GDK_WINDOWING_X11
+  if (GDK_IS_X11_DISPLAY (gtk_widget_get_display (widget)) && g_getenv("GTK_CSD") == FALSE)
+    {
+      gtk_style_context_remove_class (gtk_widget_get_style_context (widget), GTK_STYLE_CLASS_CSD);
+      gtk_style_context_remove_class (gtk_widget_get_style_context (widget), "solid-csd");
+      priv->client_decorated = FALSE;
+    }
+#endif
 }
 
 static void
@@ -4199,6 +4209,8 @@ on_titlebar_title_notify (GtkHeaderBar *titlebar,
   const gchar *title;
 
   title = gtk_header_bar_get_title (titlebar);
+  if (gtk_header_bar_get_subtitle (titlebar))
+    title = g_strconcat (title, " — ", gtk_header_bar_get_subtitle (titlebar), NULL);
   gtk_window_set_title_internal (self, title, FALSE);
 }
 
@@ -4376,9 +4388,7 @@ gtk_window_get_decorated (GtkWindow *window)
 gboolean
 gtk_window_get_decorated (GtkWindow *window)
 {
-  g_return_val_if_fail (GTK_IS_WINDOW (window), TRUE);
-
-  return window->priv->decorated;
+  return FALSE;
 }
 
 /**
@@ -7403,6 +7413,16 @@ gtk_window_realize (GtkWidget *widget)
   window = GTK_WINDOW (widget);
   priv = window->priv;
 
+  if (g_getenv ("GTK_RELAX_RGBA") != FALSE) {
+    GdkScreen *screen;
+    GdkVisual *visual;
+    screen = gtk_widget_get_screen (widget);
+    visual = gdk_screen_get_rgba_visual (screen);
+
+    if (gdk_screen_is_composited (screen) && visual != NULL)
+        gtk_widget_set_visual (widget, visual);
+  }
+
   if (!priv->client_decorated && gtk_window_should_use_csd (window))
     create_decoration (widget);
 
@@ -8053,7 +8073,7 @@ gtk_window_state_event (GtkWidget           *widget,
   GtkWindow *window = GTK_WINDOW (widget);
   GtkWindowPrivate *priv = window->priv;
 
-  if (event->changed_mask & GDK_WINDOW_STATE_FOCUSED)
+  if ((event->changed_mask & GDK_WINDOW_STATE_FOCUSED) && (priv->client_decorated || (g_strcmp0 (g_getenv ("GTK_BACKDROP"), "1") == 0)))
     ensure_state_flag_backdrop (widget);
 
   if (event->changed_mask & GDK_WINDOW_STATE_FULLSCREEN)
@@ -9402,6 +9422,7 @@ gtk_window_do_popup (GtkWindow      *window,
 gtk_window_do_popup (GtkWindow      *window,
                      GdkEventButton *event)
 {
+  if (!window->priv->client_decorated) return;
   if (!gdk_window_show_window_menu (_gtk_widget_get_window (GTK_WIDGET (window)),
                                     (GdkEvent *) event))
     gtk_window_do_popup_fallback (window, event);
