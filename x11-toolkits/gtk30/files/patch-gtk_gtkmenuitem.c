--- gtk/gtkmenuitem.c.orig	2024-01-24 01:14:34 UTC
+++ gtk/gtkmenuitem.c
@@ -1012,6 +1012,20 @@ gtk_menu_item_new_with_mnemonic (const gchar *label)
 GtkWidget*
 gtk_menu_item_new_with_mnemonic (const gchar *label)
 {
+  gchar *use_this_icon = NULL;
+  if (g_strcmp0 (label, _("Copy _Link Address")) == 0 || g_strcmp0 (label, _("_Copy")) == 0
+    || g_strcmp0 (label, _("Copy URL")) == 0)        use_this_icon = "gtk-copy";
+  else if (g_strcmp0 (label, _("Cu_t")) == 0)        use_this_icon = "gtk-cut";
+  else if (g_strcmp0 (label, _("_Paste")) == 0)      use_this_icon = "gtk-paste";
+  else if (g_strcmp0 (label, _("_Delete")) == 0)     use_this_icon = "gtk-delete";
+  else if (g_strcmp0 (label, _("Select _All")) == 0) use_this_icon = "gtk-select-all";
+  else if (g_strcmp0 (label, _("_Open Link")) == 0)  use_this_icon = "gtk-jump-to";
+  if (use_this_icon)
+    return g_object_new (GTK_TYPE_IMAGE_MENU_ITEM,
+                         "use-underline", TRUE,
+                         "label", label,
+                         "image", gtk_image_new_from_icon_name (use_this_icon, GTK_ICON_SIZE_MENU),
+                         NULL);
   return g_object_new (GTK_TYPE_MENU_ITEM,
                        "use-underline", TRUE,
                        "label", label,
