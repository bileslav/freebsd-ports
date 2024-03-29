--- gtk/gtkfilechooserwidget.c.orig	2024-01-24 01:14:34 UTC
+++ gtk/gtkfilechooserwidget.c
@@ -29,6 +29,7 @@
 #include "gtkcellrendererpixbuf.h"
 #include "gtkcellrenderertext.h"
 #include "gtkcheckmenuitem.h"
+#include "deprecated/gtkimagemenuitem.h"
 #include "gtkclipboard.h"
 #include "gtkcomboboxtext.h"
 #include "gtkdragsource.h"
@@ -80,6 +81,7 @@
 #include "gtkseparator.h"
 #include "gtkmodelbutton.h"
 #include "gtkgesturelongpress.h"
+#include "gtkcssprovider.h"
 
 #include <cairo-gobject.h>
 
@@ -1426,7 +1428,7 @@ gtk_file_chooser_widget_key_press_event (GtkWidget   *
           return TRUE;
         }
     }
-  else if (gtk_search_entry_handle_event (GTK_SEARCH_ENTRY (priv->search_entry), (GdkEvent *)event))
+  else if (FALSE && gtk_search_entry_handle_event (GTK_SEARCH_ENTRY (priv->search_entry), (GdkEvent *)event))
     {
       if (priv->operation_mode != OPERATION_MODE_SEARCH)
         operation_mode_set (impl, OPERATION_MODE_SEARCH);
@@ -2284,6 +2286,10 @@ append_separator (GtkWidget *box)
 {
   GtkWidget *separator;
 
+  separator = gtk_separator_menu_item_new ();
+  gtk_widget_set_visible (GTK_WIDGET (separator), TRUE);
+  gtk_menu_shell_append (GTK_MENU_SHELL (box), separator);
+  return separator;
   separator = g_object_new (GTK_TYPE_SEPARATOR,
                             "orientation", GTK_ORIENTATION_HORIZONTAL,
                             "visible", TRUE,
@@ -2305,6 +2311,14 @@ add_button (GtkWidget   *box,
 {
   GtkWidget *item;
 
+  if (g_str_match_string ("toggle", action, TRUE))
+    item = gtk_check_menu_item_new_with_mnemonic (label);
+  else
+    item = gtk_image_menu_item_new_with_mnemonic (label);
+  g_object_set (G_OBJECT (item), "action-name", action, NULL);
+  gtk_widget_set_visible (GTK_WIDGET (item), TRUE);
+  gtk_menu_shell_append (GTK_MENU_SHELL (box), item);
+  return item;
  item = g_object_new (GTK_TYPE_MODEL_BUTTON,
                        "visible", TRUE,
                        "action-name", action,
@@ -2329,6 +2343,9 @@ file_list_build_popover (GtkFileChooserWidget *impl)
   g_object_set (box, "margin", 10, NULL);
   gtk_widget_show (box);
   gtk_container_add (GTK_CONTAINER (priv->browse_files_popover), box);
+  priv->browse_files_popover = gtk_menu_new ();
+  gtk_menu_attach_to_widget (GTK_MENU (priv->browse_files_popover), GTK_WIDGET (priv->browse_files_tree_view), NULL);
+  box = priv->browse_files_popover;
 
   priv->visit_file_item = add_button (box, _("_Visit File"), "item.visit");
   priv->open_folder_item = add_button (box, _("_Open With File Manager"), "item.open");
@@ -2345,6 +2362,12 @@ file_list_build_popover (GtkFileChooserWidget *impl)
   priv->type_column_item = add_button (box, _("Show T_ype Column"), "item.toggle-show-type");
   priv->show_time_item = add_button (box, _("Show _Time"), "item.toggle-show-time");
   priv->sort_directories_item = add_button (box, _("Sort _Folders before Files"), "item.toggle-sort-dirs-first");
+  g_object_set (priv->visit_file_item,         "image", gtk_image_new_from_icon_name ("gtk-jump-to",    GTK_ICON_SIZE_MENU), NULL);
+  g_object_set (priv->open_folder_item,        "image", gtk_image_new_from_icon_name ("gtk-open",       GTK_ICON_SIZE_MENU), NULL);
+  g_object_set (priv->copy_file_location_item, "image", gtk_image_new_from_icon_name ("gtk-copy",       GTK_ICON_SIZE_MENU), NULL);
+  g_object_set (priv->add_shortcut_item,       "image", gtk_image_new_from_icon_name ("gtk-add",        GTK_ICON_SIZE_MENU), NULL);
+  g_object_set (priv->delete_file_item,        "image", gtk_image_new_from_icon_name ("gtk-delete",     GTK_ICON_SIZE_MENU), NULL);
+  g_object_set (priv->trash_file_item,         "image", gtk_image_new_from_icon_name ("user-trash-full",GTK_ICON_SIZE_MENU), NULL);
 }
 
 /* Updates the popover for the file list, creating it if necessary */
@@ -2404,6 +2427,8 @@ file_list_show_popover (GtkFileChooserWidget *impl,
 
 
   file_list_update_popover (impl);
+  gtk_menu_popup_at_pointer (GTK_MENU (priv->browse_files_popover), NULL);
+  return;
 
   selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (priv->browse_files_tree_view));
   list = gtk_tree_selection_get_selected_rows (selection, &model);
@@ -2496,7 +2521,7 @@ file_list_set_sort_column_ids (GtkFileChooserWidget *i
 {
   GtkFileChooserWidgetPrivate *priv = impl->priv;
 
-  gtk_tree_view_set_search_column (GTK_TREE_VIEW (priv->browse_files_tree_view), -1);
+  gtk_tree_view_set_search_column (GTK_TREE_VIEW (priv->browse_files_tree_view), 0);
 
   gtk_tree_view_column_set_sort_column_id (priv->list_name_column, MODEL_COL_NAME);
   gtk_tree_view_column_set_sort_column_id (priv->list_time_column, MODEL_COL_TIME);
@@ -3294,7 +3319,13 @@ update_appearance (GtkFileChooserWidget *impl)
 update_appearance (GtkFileChooserWidget *impl)
 {
   GtkFileChooserWidgetPrivate *priv = impl->priv;
+  gboolean eject_button = TRUE;
 
+  g_object_get (gtk_widget_get_settings (GTK_WIDGET (impl)),
+                "gtk-file-chooser-eject-button", &eject_button,
+                NULL);
+  gtk_places_sidebar_set_show_mount_eject(GTK_PLACES_SIDEBAR (priv->places_sidebar), eject_button);
+
   if (priv->action == GTK_FILE_CHOOSER_ACTION_SAVE ||
       priv->action == GTK_FILE_CHOOSER_ACTION_CREATE_FOLDER)
     {
@@ -3314,7 +3345,6 @@ update_appearance (GtkFileChooserWidget *impl)
            priv->action == GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER)
     {
       save_widgets_destroy (impl);
-      gtk_places_sidebar_set_show_recent (GTK_PLACES_SIDEBAR (priv->places_sidebar), recent_files_setting_is_enabled (impl));
       location_mode_set (impl, priv->location_mode);
     }
 
@@ -7352,8 +7382,6 @@ search_engine_hits_added_cb (GtkSearchEngine      *eng
   GFile *file;
   gboolean select = FALSE;
 
-  if (gtk_tree_model_iter_n_children (GTK_TREE_MODEL (impl->priv->search_model), NULL) == 0)
-    select = TRUE;
 
   files = NULL;
   files_with_info = NULL;
@@ -7698,7 +7726,7 @@ recent_idle_cleanup (gpointer data)
 
   gtk_tree_view_set_model (GTK_TREE_VIEW (priv->browse_files_tree_view),
                            GTK_TREE_MODEL (priv->recent_model));
-  gtk_tree_view_set_search_column (GTK_TREE_VIEW (priv->browse_files_tree_view), -1);
+  gtk_tree_view_set_search_column (GTK_TREE_VIEW (priv->browse_files_tree_view), 0);
 
   gtk_tree_view_column_set_sort_column_id (priv->list_name_column, -1);
   gtk_tree_view_column_set_sort_column_id (priv->list_time_column, -1);
@@ -8651,10 +8679,6 @@ gtk_file_chooser_widget_class_init (GtkFileChooserWidg
                                 "search-shortcut",
                                 0);
   gtk_binding_entry_add_signal (binding_set,
-                                GDK_KEY_f, GDK_CONTROL_MASK,
-                                "search-shortcut",
-                                0);
-  gtk_binding_entry_add_signal (binding_set,
                                 GDK_KEY_r, GDK_MOD1_MASK,
                                 "recent-shortcut",
                                 0);
@@ -8776,6 +8800,7 @@ post_process_ui (GtkFileChooserWidget *impl)
   g_object_set_data (G_OBJECT (impl->priv->browse_files_tree_view), I_("GtkFileChooserWidget"), impl);
 
   /* Setup file list treeview */
+  gtk_tree_view_set_enable_search (GTK_TREE_VIEW (impl->priv->browse_files_tree_view), TRUE);
   selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (impl->priv->browse_files_tree_view));
   gtk_tree_selection_set_select_function (selection,
                                           list_select_func,
@@ -8833,6 +8858,16 @@ post_process_ui (GtkFileChooserWidget *impl)
   gtk_popover_set_relative_to (GTK_POPOVER (impl->priv->rename_file_popover), impl->priv->browse_files_tree_view);
 
   add_actions (impl);
+  static gboolean style_added = FALSE;
+  if (!style_added)
+    {
+      style_added = TRUE;
+      GtkCssProvider *provider = gtk_css_provider_new ();
+      gtk_css_provider_load_from_data (provider,
+        "filechooser .sidebar-icon, filechooser .path-bar, filechooser .path-bar + button { opacity: 1; -gtk-icon-style: regular; }", -1, NULL);
+      gtk_style_context_add_provider_for_screen (gtk_widget_get_screen (GTK_WIDGET (impl)),
+        GTK_STYLE_PROVIDER (provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
+    }
 }
 
 void
