--- gtk/gtksidebarrow.c.orig	2024-01-24 01:14:34 UTC
+++ gtk/gtksidebarrow.c
@@ -230,7 +230,7 @@ gtk_sidebar_row_set_property (GObject      *object,
             self->start_icon = G_ICON (g_object_ref (object));
             gtk_image_set_from_gicon (GTK_IMAGE (self->start_icon_widget),
                                       self->start_icon,
-                                      GTK_ICON_SIZE_MENU);
+                                      GTK_ICON_SIZE_LARGE_TOOLBAR);
           }
         else
           {
@@ -415,7 +415,7 @@ gtk_sidebar_row_set_start_icon (GtkSidebarRow *self,
       g_set_object (&self->start_icon, icon);
       if (self->start_icon != NULL)
         gtk_image_set_from_gicon (GTK_IMAGE (self->start_icon_widget), self->start_icon,
-                                  GTK_ICON_SIZE_MENU);
+                                  GTK_ICON_SIZE_LARGE_TOOLBAR);
       else
         gtk_image_clear (GTK_IMAGE (self->start_icon_widget));
 
