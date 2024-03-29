--- gdk/wayland/gdkscreen-wayland.c.orig	2024-01-24 01:14:34 UTC
+++ gdk/wayland/gdkscreen-wayland.c
@@ -1093,12 +1093,6 @@ gdk_wayland_screen_get_setting (GdkScreen   *screen,
     return set_capability_setting (screen, value,
                                    GTK_SHELL1_CAPABILITY_DESKTOP_ICONS);
 
-  if (strcmp (name, "gtk-dialogs-use-header") == 0)
-    {
-      g_value_set_boolean (value, TRUE);
-      return TRUE;
-    }
-
   if (strcmp (name, "gtk-fontconfig-timestamp") == 0)
     {
       g_value_set_uint (value, wayland_screen->dbus_settings.fontconfig_timestamp);
