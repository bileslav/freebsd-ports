--- gtk/gtkplacessidebar.c.orig	2024-01-24 01:14:34 UTC
+++ gtk/gtkplacessidebar.c
@@ -64,6 +64,7 @@
 #include "gtkgesturelongpress.h"
 #include "gtkbox.h"
 #include "gtkmodelbutton.h"
+#include "deprecated/gtkimagemenuitem.h"
 
 /**
  * SECTION:gtkplacessidebar
@@ -191,6 +192,7 @@ struct _GtkPlacesSidebar {
   guint show_starred_location  : 1;
   guint local_only             : 1;
   guint populate_all           : 1;
+  guint show_eject             : 1;
 };
 
 struct _GtkPlacesSidebarClass {
@@ -261,6 +263,7 @@ enum {
   PROP_LOCAL_ONLY,
   PROP_SHOW_OTHER_LOCATIONS,
   PROP_POPULATE_ALL,
+  PROP_SHOW_MOUNT_EJECT,
   NUM_PROPERTIES
 };
 
@@ -454,12 +457,34 @@ list_box_header_func (GtkListBoxRow *row,
   else
     {
       before_section_type = SECTION_INVALID;
-      gtk_widget_set_margin_top (GTK_WIDGET (row), 4);
     }
 
-  if (before && before_section_type != row_section_type)
+  if (before_section_type != row_section_type)
     {
-      separator = gtk_separator_new (GTK_ORIENTATION_HORIZONTAL);
+      switch (row_section_type)
+        {
+        case SECTION_COMPUTER:
+        case SECTION_BOOKMARKS:
+          if (before_section_type == SECTION_COMPUTER)
+            {
+              return;
+            }
+          separator = gtk_label_new(NULL);
+          gtk_label_set_markup(GTK_LABEL(separator), "<b>Places</b>");
+          break;
+        case SECTION_MOUNTS:
+          separator = gtk_label_new(NULL);
+          gtk_label_set_markup(GTK_LABEL(separator), "<b>Devices</b>");
+          break;
+        case SECTION_OTHER_LOCATIONS:
+          separator = gtk_label_new(NULL);
+          gtk_label_set_markup(GTK_LABEL(separator), "<b>Network</b>");
+          break;
+        default:
+          separator = gtk_separator_new (GTK_ORIENTATION_HORIZONTAL);
+        }
+      gtk_widget_set_halign(separator, GTK_ALIGN_START);
+      gtk_widget_set_margin_left (separator, 14);
       gtk_widget_set_margin_top (separator, 4);
       gtk_widget_set_margin_bottom (separator, 4);
       gtk_list_box_row_set_header (row, separator);
@@ -497,7 +522,7 @@ add_place (GtkPlacesSidebar            *sidebar,
   if (show_unmount || show_eject)
     g_assert (place_type != PLACES_BOOKMARK);
 
-  show_eject_button = (show_unmount || show_eject);
+  show_eject_button = (sidebar->show_eject && (show_unmount || show_eject));
 
   row = g_object_new (GTK_TYPE_SIDEBAR_ROW,
                       "sidebar", sidebar,
@@ -528,6 +553,7 @@ add_place (GtkPlacesSidebar            *sidebar,
   g_signal_connect (event_box, "button-release-event",
                     G_CALLBACK (on_button_release_event), row);
 
+  gtk_widget_set_margin_left(GTK_WIDGET (row), 10);
   gtk_container_add (GTK_CONTAINER (sidebar->list_box), GTK_WIDGET (row));
   gtk_widget_show_all (row);
 
@@ -633,6 +659,7 @@ add_special_dirs (GtkPlacesSidebar *sidebar)
 static void
 add_special_dirs (GtkPlacesSidebar *sidebar)
 {
+  return;
   GList *dirs;
   gint index;
 
@@ -1096,7 +1123,7 @@ update_places (GtkPlacesSidebar *sidebar)
              SECTION_COMPUTER,
              _("Home"), start_icon, NULL, home_uri,
              NULL, NULL, NULL, NULL, 0,
-             _("Open your personal folder"));
+             _("Open the home folder"));
   g_object_unref (start_icon);
   g_free (home_uri);
 
@@ -1371,7 +1398,7 @@ update_places (GtkPlacesSidebar *sidebar)
                  SECTION_MOUNTS,
                  sidebar->hostname, start_icon, NULL, mount_uri,
                  NULL, NULL, NULL, NULL, 0,
-                 _("Open the contents of the file system"));
+                 _("Browse the file system"));
       g_object_unref (start_icon);
     }
 
@@ -1430,8 +1457,6 @@ update_places (GtkPlacesSidebar *sidebar)
       root = sl->data;
       is_native = g_file_is_native (root);
 
-      if (_gtk_bookmarks_manager_get_is_builtin (sidebar->bookmarks_manager, root))
-        continue;
 
       if (sidebar->local_only && !is_native)
         continue;
@@ -1539,7 +1564,18 @@ update_places (GtkPlacesSidebar *sidebar)
 
       g_object_unref (start_icon);
     }
+  else if (!sidebar->local_only)
+    {
+      start_icon = g_themed_icon_new_with_default_fallbacks (ICON_NAME_NETWORK);
 
+      add_place (sidebar, PLACES_BUILT_IN,
+                 SECTION_OTHER_LOCATIONS,
+                 _("Browse Network"), start_icon, NULL, "network:///",
+                 NULL, NULL, NULL, NULL, 0, _("Browse local network connections"));
+
+      g_object_unref (start_icon);
+    }
+
   gtk_widget_show_all (GTK_WIDGET (sidebar));
   /* We want this hidden by default, but need to do it after the show_all call */
   gtk_sidebar_row_hide (GTK_SIDEBAR_ROW (sidebar->new_bookmark_row), TRUE);
@@ -2334,32 +2370,32 @@ check_popover_sensitivity (GtkSidebarRow *row,
   gtk_widget_set_visible (data->stop_item, show_stop);
 
   /* Adjust start/stop items to reflect the type of the drive */
-  g_object_set (data->start_item, "text", _("_Start"), NULL);
-  g_object_set (data->stop_item, "text", _("_Stop"), NULL);
+  g_object_set (data->start_item, "label", _("_Start"), "use_underline", TRUE, NULL);
+  g_object_set (data->stop_item, "label", _("_Stop"), "use_underline", TRUE, NULL);
   if ((show_start || show_stop) && drive != NULL)
     {
       switch (g_drive_get_start_stop_type (drive))
         {
         case G_DRIVE_START_STOP_TYPE_SHUTDOWN:
           /* start() for type G_DRIVE_START_STOP_TYPE_SHUTDOWN is normally not used */
-          g_object_set (data->start_item, "text", _("_Power On"), NULL);
-          g_object_set (data->stop_item, "text", _("_Safely Remove Drive"), NULL);
+          g_object_set (data->start_item, "label", _("_Power On"), "use_underline", TRUE, NULL);
+          g_object_set (data->stop_item, "label", _("_Safely Remove Drive"), "use_underline", TRUE, NULL);
           break;
 
         case G_DRIVE_START_STOP_TYPE_NETWORK:
-          g_object_set (data->start_item, "text", _("_Connect Drive"), NULL);
-          g_object_set (data->stop_item, "text", _("_Disconnect Drive"), NULL);
+          g_object_set (data->start_item, "label", _("_Connect Drive"), "use_underline", TRUE, NULL);
+          g_object_set (data->stop_item, "label", _("_Disconnect Drive"), "use_underline", TRUE, NULL);
           break;
 
         case G_DRIVE_START_STOP_TYPE_MULTIDISK:
-          g_object_set (data->start_item, "text", _("_Start Multi-disk Device"), NULL);
-          g_object_set (data->stop_item, "text", _("_Stop Multi-disk Device"), NULL);
+          g_object_set (data->start_item, "label", _("_Start Multi-disk Device"), "use_underline", TRUE, NULL);
+          g_object_set (data->stop_item, "label", _("_Stop Multi-disk Device"), "use_underline", TRUE, NULL);
           break;
 
         case G_DRIVE_START_STOP_TYPE_PASSWORD:
           /* stop() for type G_DRIVE_START_STOP_TYPE_PASSWORD is normally not used */
-          g_object_set (data->start_item, "text", _("_Unlock Device"), NULL);
-          g_object_set (data->stop_item, "text", _("_Lock Device"), NULL);
+          g_object_set (data->start_item, "label", _("_Unlock Device"), "use_underline", TRUE, NULL);
+          g_object_set (data->stop_item, "label", _("_Lock Device"), "use_underline", TRUE, NULL);
           break;
 
         default:
@@ -3552,6 +3588,13 @@ append_separator (GtkWidget *box)
 {
   GtkWidget *separator;
 
+  if (GTK_IS_MENU (box))
+    {
+      separator = gtk_separator_menu_item_new();
+      gtk_widget_set_visible(GTK_WIDGET(separator), TRUE);
+      gtk_menu_shell_append(GTK_MENU_SHELL(box), separator);
+      return separator;
+    }
   separator = g_object_new (GTK_TYPE_SEPARATOR,
                             "orientation", GTK_ORIENTATION_HORIZONTAL,
                             "visible", TRUE,
@@ -3570,6 +3613,14 @@ add_button (GtkWidget   *box,
 {
   GtkWidget *item;
 
+  if (GTK_IS_MENU (box))
+    {
+      item = gtk_image_menu_item_new_with_mnemonic(label);
+      g_object_set(G_OBJECT(item), "action-name", action, NULL);
+      gtk_widget_set_visible(GTK_WIDGET(item), TRUE);
+      gtk_menu_shell_append(GTK_MENU_SHELL(box), item);
+      return item;
+    }
   item = g_object_new (GTK_TYPE_MODEL_BUTTON,
                        "visible", TRUE,
                        "action-name", action,
@@ -3587,6 +3638,17 @@ add_open_button (GtkWidget          *box,
 {
   GtkWidget *item;
 
+  if (GTK_IS_MENU (box))
+    {
+      item = gtk_image_menu_item_new_with_mnemonic (label);
+      g_object_set(G_OBJECT (item),
+        "action-name", flags == GTK_PLACES_OPEN_NORMAL ? "row.open" : "row.open-other",
+        "action-target", g_variant_new_int32 (flags),
+      NULL);
+      gtk_widget_set_visible (GTK_WIDGET (item), TRUE);
+      gtk_menu_shell_append (GTK_MENU_SHELL (box), item);
+      return item;
+    }
   item = g_object_new (GTK_TYPE_MODEL_BUTTON,
                        "visible", TRUE,
                        "action-name", flags == GTK_PLACES_OPEN_NORMAL ? "row.open" : "row.open-other",
@@ -3692,6 +3754,13 @@ create_row_popover (GtkPlacesSidebar *sidebar,
   g_object_set (box, "margin", 10, NULL);
   gtk_widget_show (box);
   gtk_container_add (GTK_CONTAINER (sidebar->popover), box);
+  if (!sidebar->populate_all)
+    {
+      sidebar->popover = gtk_menu_new ();
+      g_signal_connect (sidebar->popover, "destroy", G_CALLBACK (on_row_popover_destroy), sidebar);
+      gtk_menu_attach_to_widget (GTK_MENU (sidebar->popover), GTK_WIDGET (sidebar), NULL);
+      box = sidebar->popover;
+    }
 
   add_open_button (box, _("_Open"), GTK_PLACES_OPEN_NORMAL);
 
@@ -3715,6 +3784,15 @@ create_row_popover (GtkPlacesSidebar *sidebar,
   data.rescan_item = add_button (box, _("_Detect Media"), "row.rescan");
   data.start_item = add_button (box, _("_Start"), "row.start");
   data.stop_item = add_button (box, _("_Stop"), "row.stop");
+  if (!sidebar->populate_all)
+    {
+      g_object_set (
+        g_list_first (gtk_container_get_children (GTK_CONTAINER (box)))->data,
+        "image", gtk_image_new_from_icon_name ("gtk-open", GTK_ICON_SIZE_MENU), NULL
+      );
+      g_object_set (data.remove_item,       "image", gtk_image_new_from_icon_name("gtk-remove",GTK_ICON_SIZE_MENU), NULL);
+      g_object_set (data.add_shortcut_item, "image", gtk_image_new_from_icon_name("gtk-add",   GTK_ICON_SIZE_MENU), NULL);
+    }
 
   /* Update everything! */
   check_popover_sensitivity (row, &data);
@@ -3758,6 +3836,13 @@ show_row_popover (GtkSidebarRow *row)
     gtk_widget_destroy (sidebar->popover);
 
   create_row_popover (sidebar, row);
+  if (GTK_IS_MENU (sidebar->popover))
+    {
+      sidebar->context_row = row;
+      gtk_menu_popup_at_pointer (GTK_MENU (sidebar->popover), NULL);
+      g_object_unref (sidebar);
+      return;
+    }
 
   gtk_popover_set_relative_to (GTK_POPOVER (sidebar->popover), GTK_WIDGET (row));
 
@@ -3914,7 +3999,15 @@ list_box_sort_func (GtkListBoxRow *row1,
                place_type_1 == PLACES_XDG_DIR) ||
               section_type_1 == SECTION_MOUNTS)
             {
-              retval = g_utf8_collate (label_1, label_2);
+              /* Local non-removable in first mounts position */
+              if ((section_type_1 == SECTION_MOUNTS) &&
+                  (place_type_1 == PLACES_BUILT_IN))
+                retval = -1;
+              else if ((section_type_1 == SECTION_MOUNTS) &&
+                       (place_type_2 == PLACES_BUILT_IN))
+                retval = 1;
+              else
+                retval = g_utf8_collate (label_1, label_2);
             }
           else if ((place_type_1 == PLACES_BOOKMARK || place_type_2 == PLACES_DROP_FEEDBACK) &&
                    (place_type_1 == PLACES_DROP_FEEDBACK || place_type_2 == PLACES_BOOKMARK))
@@ -4081,6 +4174,7 @@ gtk_places_sidebar_init (GtkPlacesSidebar *sidebar)
   sidebar->cancellable = g_cancellable_new ();
 
   sidebar->show_trash = TRUE;
+  sidebar->show_eject = TRUE;
 
   create_volume_monitor (sidebar);
 
@@ -4158,7 +4252,7 @@ gtk_places_sidebar_init (GtkPlacesSidebar *sidebar)
 
   gtk_container_add (GTK_CONTAINER (sidebar), sidebar->list_box);
 
-  sidebar->hostname = g_strdup (_("Computer"));
+  sidebar->hostname = g_strdup (_("File System"));
   sidebar->hostnamed_cancellable = g_cancellable_new ();
   g_dbus_proxy_new_for_bus (G_BUS_TYPE_SYSTEM,
                             G_DBUS_PROXY_FLAGS_GET_INVALIDATED_PROPERTIES,
@@ -4254,6 +4348,10 @@ G_GNUC_END_IGNORE_DEPRECATIONS
         }
       break;
 
+    case PROP_SHOW_MOUNT_EJECT:
+      gtk_places_sidebar_set_show_mount_eject(sidebar, g_value_get_boolean (value));
+      break;
+
     default:
       G_OBJECT_WARN_INVALID_PROPERTY_ID (obj, property_id, pspec);
       break;
@@ -4316,6 +4414,10 @@ G_GNUC_END_IGNORE_DEPRECATIONS
       g_value_set_boolean (value, sidebar->populate_all);
       break;
 
+    case PROP_SHOW_MOUNT_EJECT:
+      g_value_set_boolean(value, gtk_places_sidebar_get_show_mount_eject(sidebar));
+      break;
+
     default:
       G_OBJECT_WARN_INVALID_PROPERTY_ID (obj, property_id, pspec);
       break;
@@ -4858,6 +4960,12 @@ gtk_places_sidebar_class_init (GtkPlacesSidebarClass *
                                 P_("Whether the sidebar includes an item to show starred files"),
                                 FALSE,
                                 G_PARAM_READWRITE);
+  properties[PROP_SHOW_MOUNT_EJECT] =
+          g_param_spec_boolean ("show-mount-eject",
+                                P_("Show mount eject button"),
+                                P_("Whether the sidebar includes mount eject buttons"),
+                                TRUE,
+                                G_PARAM_READWRITE);
 
 
   /**
@@ -5658,4 +5766,46 @@ gtk_places_sidebar_get_show_starred_location (GtkPlace
   g_return_val_if_fail (GTK_IS_PLACES_SIDEBAR (sidebar), FALSE);
 
   return sidebar->show_starred_location;
-}
+}
+
+/**
+ * gtk_places_sidebar_set_show_mount_eject:
+ * @sidebar: a places sidebar
+ * @show_eject: whether to show mount eject buttons
+ *
+ * Sets whether the @sidebar should show mount eject buttons.
+ *
+ * Since: 3.22.37
+ */
+void
+gtk_places_sidebar_set_show_mount_eject (GtkPlacesSidebar *sidebar,
+                                         gboolean         show_eject)
+{
+  g_return_if_fail (GTK_IS_PLACES_SIDEBAR (sidebar));
+
+  show_eject = !!show_eject;
+  if (sidebar->show_eject != show_eject)
+    {
+      sidebar->show_eject = show_eject;
+      update_places (sidebar);
+      g_object_notify_by_pspec (G_OBJECT (sidebar), properties[PROP_SHOW_MOUNT_EJECT]);
+    }
+}
+
+/**
+ * gtk_places_sidebar_get_show_mount_eject:
+ * @sidebar: a places sidebar
+ *
+ * Returns the value previously set with gtk_places_sidebar_set_show_mount_eject()
+ *
+ * Returns: %TRUE if the sidebar will display a mount eject buttons.
+ *
+ * Since: 3.22.37
+ */
+gboolean
+gtk_places_sidebar_get_show_mount_eject (GtkPlacesSidebar *sidebar)
+{
+  g_return_val_if_fail (GTK_IS_PLACES_SIDEBAR (sidebar), TRUE);
+
+  return sidebar->show_eject;
+}
\ No newline at end of file
