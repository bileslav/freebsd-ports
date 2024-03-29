--- gtk/gtkprintunixdialog.c.orig	2024-01-24 01:14:34 UTC
+++ gtk/gtkprintunixdialog.c
@@ -750,6 +750,11 @@ gtk_print_unix_dialog_init (GtkPrintUnixDialog *dialog
   gtk_dialog_set_default_response (GTK_DIALOG (dialog), GTK_RESPONSE_OK);
   widget = gtk_dialog_get_widget_for_response (GTK_DIALOG (dialog), GTK_RESPONSE_OK);
   gtk_widget_set_sensitive (widget, FALSE);
+  GtkCssProvider *provider = gtk_css_provider_new ();
+  gtk_css_provider_load_from_data (provider, ".dialog-action-box { margin: 0; }", -1, NULL);
+  GtkStyleContext *context = gtk_widget_get_style_context (gtk_widget_get_parent (gtk_dialog_get_action_area (GTK_DIALOG (dialog))));
+  gtk_style_context_add_provider (context, GTK_STYLE_PROVIDER (provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
+  gtk_style_context_save (context);
 
   /* Treeview auxiliary functions need to be setup here */
   gtk_tree_model_filter_set_visible_func (priv->printer_list_filter,
