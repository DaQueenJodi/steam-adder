#include "vdf.h"
#include <gtk/gtk.h>
#include <libgen.h>
#include <stdlib.h>

char *file_name = NULL;

static void add_steam_entry(GtkWidget *widget, gpointer *data) {
  (void)widget;
  (void)data;

  if (file_name == NULL) {
    g_print("Please select a file\n");
    return;
  }
  char *dir = dirname(file_name);

  VDFNode *node = vdf_deserialize("test.vdf");
	vdf_print(node);
	//	node_clean(node);
}

static void choose_file(GtkWidget *widget, gpointer data) {

  GtkWidget *file_chooser = gtk_file_chooser_dialog_new(
      "Choose an executable", GTK_WINDOW(gtk_widget_get_toplevel(widget)),
      GTK_FILE_CHOOSER_ACTION_OPEN, "Cancel", GTK_RESPONSE_CANCEL, "Open",
      GTK_RESPONSE_ACCEPT, NULL);
  gint res = gtk_dialog_run(GTK_DIALOG(file_chooser));
  if (res == GTK_RESPONSE_ACCEPT) {
    file_name = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(file_chooser));
    g_print("%s\n", file_name);
  }
  gtk_widget_destroy(file_chooser);
}

int main(void) {
  VDFNode *node = vdf_deserialize("test.vdf");
  vdf_print(node);
}

int main2(int argc, char **argv) {
  gtk_init(&argc, &argv);
  GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_container_set_border_width(GTK_CONTAINER(window), 10);
  g_signal_connect(window, "delete_event", G_CALLBACK(gtk_main_quit), NULL);

  GtkWidget *grid = gtk_grid_new();
  gtk_container_add(GTK_CONTAINER(window), grid);

  GtkWidget *button;
  button = gtk_button_new_with_label("Open Executable");
  g_signal_connect(button, "clicked", G_CALLBACK(choose_file), NULL);
  gtk_grid_attach(GTK_GRID(grid), button, 0, 0, 1, 1);

  button = gtk_button_new_with_label("Done");
  g_signal_connect(button, "clicked", G_CALLBACK(add_steam_entry), NULL);
  gtk_grid_attach(GTK_GRID(grid), button, 1, 0, 1, 1);

  gtk_widget_show_all(window);
  gtk_main();
}
