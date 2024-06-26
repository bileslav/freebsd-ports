--- gtk/gtkheaderbar.c.orig	2024-01-24 01:14:34 UTC
+++ gtk/gtkheaderbar.c
@@ -300,6 +300,27 @@ _gtk_header_bar_update_window_buttons (GtkHeaderBar *b
 
   priv->titlebar_icon = NULL;
 
+  gboolean csd_cleaned_headerbar = (!gtk_style_context_has_class (gtk_widget_get_style_context (toplevel), "csd") &&
+                                    !gtk_style_context_has_class (gtk_widget_get_style_context (toplevel), "solid-csd"));
+  if (csd_cleaned_headerbar)
+    {
+      /* if (priv->label_box != NULL)
+        {
+          priv->label_box = NULL;
+          priv->title_label = NULL;
+          priv->subtitle_label = NULL;
+        } */
+      static gboolean style_added = FALSE;
+      if (!style_added)
+        {
+          style_added = TRUE;
+          GtkCssProvider *provider = gtk_css_provider_new ();
+          gtk_css_provider_load_from_data (provider,
+            "headerbar, .titlebar { border-radius: 0; border-top: 0; box-shadow: none; } headerbar .title { color: transparent; } headerbar button .title { color: initial; }", -1, NULL);
+          gtk_style_context_add_provider_for_screen (gtk_widget_get_screen (widget),
+            GTK_STYLE_PROVIDER (provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
+        }
+    }
   if (!priv->shows_wm_decorations)
     return;
 
@@ -327,6 +348,8 @@ _gtk_header_bar_update_window_buttons (GtkHeaderBar *b
                           gtk_window_get_transient_for (window) == NULL &&
                           gtk_window_get_type_hint (window) == GDK_WINDOW_TYPE_HINT_NORMAL);
 
+  if (csd_cleaned_headerbar)
+      layout_desc = strstr (layout_desc, "menu") ? g_strdup ("menu") : g_strdup ("");
   tokens = g_strsplit (layout_desc, ":", 2);
   if (tokens)
     {
@@ -391,9 +414,9 @@ _gtk_header_bar_update_window_buttons (GtkHeaderBar *b
                     atk_object_set_name (accessible, _("Application menu"));
 
                   priv->titlebar_icon = image;
-                  if (!_gtk_header_bar_update_window_icon (bar, window))
+                  if (csd_cleaned_headerbar || !_gtk_header_bar_update_window_icon (bar, window))
                     gtk_image_set_from_icon_name (GTK_IMAGE (priv->titlebar_icon),
-                                                  "application-x-executable-symbolic", GTK_ICON_SIZE_MENU);
+                                                  "pan-down-symbolic", GTK_ICON_SIZE_MENU);
                 }
               else if (strcmp (t[j], "minimize") == 0 &&
                        is_sovereign_window)
@@ -1361,6 +1384,7 @@ gtk_header_bar_set_subtitle (GtkHeaderBar *bar,
   gtk_widget_set_visible (priv->subtitle_sizing_label, priv->has_subtitle || (priv->subtitle && priv->subtitle[0]));
 
   g_object_notify_by_pspec (G_OBJECT (bar), header_bar_props[PROP_SUBTITLE]);
+  g_object_notify_by_pspec (G_OBJECT (bar), header_bar_props[PROP_TITLE]);
 }
 
 /**
