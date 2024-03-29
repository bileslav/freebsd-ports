--- gtk/gtkplacessidebar.h.orig	2024-01-24 01:14:34 UTC
+++ gtk/gtkplacessidebar.h
@@ -155,6 +155,12 @@ gboolean             gtk_places_sidebar_get_show_starr
                                                                    gboolean            show_starred_location);
 GDK_AVAILABLE_IN_3_22
 gboolean             gtk_places_sidebar_get_show_starred_location (GtkPlacesSidebar   *sidebar);
+
+GDK_AVAILABLE_IN_3_24
+gboolean           gtk_places_sidebar_get_show_mount_eject        (GtkPlacesSidebar   *sidebar);
+GDK_AVAILABLE_IN_3_24
+void               gtk_places_sidebar_set_show_mount_eject        (GtkPlacesSidebar   *sidebar,
+                                                                   gboolean           show_eject);
 G_END_DECLS
 
 #endif /* __GTK_PLACES_SIDEBAR_H__ */
