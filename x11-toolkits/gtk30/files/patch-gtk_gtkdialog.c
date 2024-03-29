--- gtk/gtkdialog.c.orig	2024-01-24 01:14:34 UTC
+++ gtk/gtkdialog.c
@@ -701,12 +701,12 @@ update_spacings (GtkDialog *dialog)
 
   /* don't set spacing when buttons are linked */
   if (gtk_button_box_get_layout (GTK_BUTTON_BOX (priv->action_area)) != GTK_BUTTONBOX_EXPAND)
-    gtk_box_set_spacing (GTK_BOX (priv->action_area), button_spacing);
+    gtk_box_set_spacing (GTK_BOX (priv->action_area), (button_spacing == 0) ? 6 : button_spacing);
 
   if (!_gtk_container_get_border_width_set (GTK_CONTAINER (priv->action_area)))
     {
       gtk_container_set_border_width (GTK_CONTAINER (priv->action_area),
-                                      action_area_border);
+                                      (action_area_border == 0) ? 5 : action_area_border);
       _gtk_container_set_border_width_set (GTK_CONTAINER (priv->action_area), FALSE);
     }
 }
