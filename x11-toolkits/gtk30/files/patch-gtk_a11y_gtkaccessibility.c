--- gtk/a11y/gtkaccessibility.c.orig	2024-01-24 01:14:34 UTC
+++ gtk/a11y/gtkaccessibility.c
@@ -38,8 +38,10 @@
 #include <gtk/gtkaccessible.h>
 
 #ifdef GDK_WINDOWING_X11
+#ifdef HAVE_ATK_BRIDGE
 #include <atk-bridge.h>
 #endif
+#endif
 
 static gboolean gail_focus_watcher      (GSignalInvocationHint *ihint,
                                          guint                  n_param_values,
@@ -989,7 +991,9 @@ _gtk_accessibility_init (void)
   do_window_event_initialization ();
 
 #ifdef GDK_WINDOWING_X11
+#ifdef HAVE_ATK_BRIDGE
   atk_bridge_adaptor_init (NULL, NULL);
+#endif
 #endif
 
   atk_misc_instance = g_object_new (GTK_TYPE_MISC_IMPL, NULL);
