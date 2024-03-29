--- gtk/gtkplacessidebarprivate.h.orig	2024-01-24 01:14:34 UTC
+++ gtk/gtkplacessidebarprivate.h
@@ -30,9 +30,9 @@ typedef enum {
 typedef enum {
   SECTION_INVALID,
   SECTION_COMPUTER,
+  SECTION_BOOKMARKS,
   SECTION_MOUNTS,
   SECTION_CLOUD,
-  SECTION_BOOKMARKS,
   SECTION_OTHER_LOCATIONS,
   N_SECTIONS
 } GtkPlacesSidebarSectionType;
