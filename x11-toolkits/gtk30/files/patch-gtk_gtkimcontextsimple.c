--- gtk/gtkimcontextsimple.c.orig	2024-01-24 01:14:34 UTC
+++ gtk/gtkimcontextsimple.c
@@ -1135,7 +1135,7 @@ gtk_im_context_simple_get_preedit_string (GtkIMContext
                   gunichar ch;
                   gboolean need_space;
 
-                  if (is_dead_key (priv->compose_buffer[i]))
+                  if (FALSE && is_dead_key (priv->compose_buffer[i]))
                     {
                       ch = dead_key_to_unicode (priv->compose_buffer[i], &need_space);
                       if (ch)
