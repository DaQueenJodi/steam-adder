#include "shortcuts.h"
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

  //  VDFNode *node = vdf_deserialize("test.vdf");
  // vdf_print_children(node);
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
  // VDFNode *node = vdf_deserialize("test.vdf");
  // vdf_print_children(node, 0);
  //  vdf_clean(node);

  Shortcut s;
  s.appid = 2871305054;
  s.app_name = "RainWorld.exe";
  s.exe = "/hdd/Documents/NotPiracy/Games/Rain.World/Game/files/RainWorld.exe";
  s.start_dir = "/hdd/Documents/NotPiracy/Games/Rain.World/games/files/";
  s.icon = "";
  s.shortcut_path = "";
  s.launch_options = "";
  s.is_hidden = 0;
  s.allow_desktop_config = 1;
  s.allow_overlay = 1;
  s.openvr = 0;
  s.devkit = 0;
  s.devkit_game_id = "";
  s.devkit_override_app_id = 0;
  s.last_play_time = 0;
  s.flatpak_app_id = "";
  char *tags[] = {"favorite"};
  s.tags = (Tags){.tags = tags, .size = 1};

  Shortcuts ss;
  ss.buff = malloc(sizeof(Shortcut *));
  ss.buff[0] = &s;
  ss.capacity = 1;
  ss.len = 1;

	VDFNode *n = shortcuts_to_vdf(&ss);
	vdf_print_children(n, 0);
	vdf_serialize(n, "out.vdf");
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

	return 0;
}
