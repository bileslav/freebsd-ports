--- gtk/gtksettings.c.orig	2024-01-24 01:14:34 UTC
+++ gtk/gtksettings.c
@@ -61,7 +61,7 @@
 #ifdef GDK_WINDOWING_QUARTZ
 #define PRINT_PREVIEW_COMMAND "open -b com.apple.Preview %f"
 #else
-#define PRINT_PREVIEW_COMMAND "evince --unlink-tempfile --preview --print-settings %s %f"
+#define PRINT_PREVIEW_COMMAND "atril --unlink-tempfile --preview --print-settings %s %f"
 #endif
 
 /**
@@ -226,7 +226,8 @@ enum {
   PROP_RECENT_FILES_ENABLED,
   PROP_LONG_PRESS_TIME,
   PROP_KEYNAV_USE_CARET,
-  PROP_OVERLAY_SCROLLING
+  PROP_OVERLAY_SCROLLING,
+  PROP_FILE_CHOOSER_EJECT_BUTTON
 };
 
 /* --- prototypes --- */
@@ -1796,6 +1797,24 @@ gtk_settings_class_init (GtkSettingsClass *class)
                                                                    GTK_PARAM_READWRITE),
                                              NULL);
   g_assert (result == PROP_OVERLAY_SCROLLING);
+
+  /**
+   * GtkSettings:gtk-file-chooser-eject-button:
+   *
+   * Whether a mount/eject button is visible for mount entries within the
+   * file-chooser places sidebar widget. If this is set to %FALSE, a mount/eject
+   * button will not be visible for mount entries.
+   *
+   * Since: 3.24.37
+   */
+  result = settings_install_property_parser (class,
+                                             g_param_spec_boolean ("gtk-file-chooser-eject-button",
+                                                                   P_("Whether file-chooser mounts include a mount/eject button"),
+                                                                   P_("Whether file-chooser mounts include a mount/eject button"),
+                                                                   TRUE,
+                                                                   GTK_PARAM_READWRITE),
+                                             NULL);
+  g_assert (result == PROP_FILE_CHOOSER_EJECT_BUTTON);
 }
 
 static void
