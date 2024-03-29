--- gtk/gtkmessagedialog.c.orig	2024-01-24 01:14:34 UTC
+++ gtk/gtkmessagedialog.c
@@ -37,6 +37,7 @@
 #include "gtkintl.h"
 #include "gtkprivate.h"
 #include "gtktypebuiltins.h"
+#include "gtkcssprovider.h"
 
 /**
  * SECTION:gtkmessagedialog
@@ -327,7 +328,17 @@ G_GNUC_END_IGNORE_DEPRECATIONS
 G_GNUC_BEGIN_IGNORE_DEPRECATIONS
   action_area = gtk_dialog_get_action_area (GTK_DIALOG (dialog));
 G_GNUC_END_IGNORE_DEPRECATIONS
-  gtk_button_box_set_layout (GTK_BUTTON_BOX (action_area), GTK_BUTTONBOX_EXPAND);
+  static gboolean style_added = FALSE;
+  if (!style_added)
+    {
+      style_added = TRUE;
+      GtkCssProvider *provider = gtk_css_provider_new ();
+      gtk_css_provider_load_from_data (provider,
+        "messagedialog > *, messagedialog buttonbox.dialog-action-area, messagedialog box.dialog-action-box { margin: 0; margin-top: 0; padding: 0; }",
+        -1, NULL);
+      gtk_style_context_add_provider_for_screen (gtk_widget_get_screen (GTK_WIDGET (dialog)),
+        GTK_STYLE_PROVIDER (provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
+    }
 
   settings = gtk_widget_get_settings (GTK_WIDGET (dialog));
   g_object_get (settings, "gtk-keynav-use-caret", &use_caret, NULL);
@@ -372,6 +383,7 @@ setup_type (GtkMessageDialog *dialog,
 {
   GtkMessageDialogPrivate *priv = dialog->priv;
   const gchar *name = NULL;
+  const gchar *icon = NULL;
   AtkObject *atk_obj;
 
   if (priv->message_type == type)
@@ -383,18 +395,22 @@ setup_type (GtkMessageDialog *dialog,
     {
     case GTK_MESSAGE_INFO:
       name = _("Information");
+      icon = "gtk-dialog-info";
       break;
 
     case GTK_MESSAGE_QUESTION:
       name = _("Question");
+      icon = "gtk-dialog-question";
       break;
 
     case GTK_MESSAGE_WARNING:
       name = _("Warning");
+      icon = "gtk-dialog-warning";
       break;
 
     case GTK_MESSAGE_ERROR:
       name = _("Error");
+      icon = "gtk-dialog-error";
       break;
 
     case GTK_MESSAGE_OTHER:
@@ -405,6 +421,12 @@ setup_type (GtkMessageDialog *dialog,
       break;
     }
 
+  if (icon)
+    {
+      GtkWidget *image = gtk_image_new_from_icon_name (icon, GTK_ICON_SIZE_DIALOG);
+      g_object_set (image, "visible", TRUE, NULL);
+      g_object_set (dialog, "image", image, NULL);
+    }
   atk_obj = gtk_widget_get_accessible (GTK_WIDGET (dialog));
   if (GTK_IS_ACCESSIBLE (atk_obj))
     {
