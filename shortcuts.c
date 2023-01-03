#include "shortcuts.h"
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

static void add_shortcut(Shortcuts *ss, Shortcut *s) {
  if (ss->len == ss->capacity) {
    ss->capacity *= 2;
    ss->buff = realloc(ss->buff, ss->capacity * sizeof(Shortcut *));
  }

  ss->buff[ss->len++] = s;
}

static Shortcuts *new_shortcuts(void) {
  Shortcuts *s = malloc(sizeof(Shortcuts));
  s->buff = calloc(100, sizeof(Shortcut *));
  return s;
}

#define ADD_ITEM(name, t, data_type, d)                                        \
  {                                                                            \
    VDFItem item;                                                              \
    item.k = name;                                                             \
    item.v.type = t;                                                           \
    item.v.data.data_type = d;                                                 \
    vdf_node_add_item(n, item);                                                \
  }
VDFNode *shortcuts_to_vdf(Shortcuts *s) {
  VDFNode *node = vdf_new_node();
  node->name = "shortcuts";
  for (size_t i = 0; i < s->len; i++) {

    Shortcut *shortcut = s->buff[i];

    VDFNode *n = vdf_new_node();
    n->name = calloc(1, sizeof(size_t));
    sprintf(n->name, "%zu", i);
    // clang-format off
    ADD_ITEM("appid", PACKTYPE_INT, u32, shortcut->appid);
    ADD_ITEM("AppName", PACKTYPE_STRING, string, shortcut->app_name);
    ADD_ITEM("Exe", PACKTYPE_STRING, string, shortcut->exe);
		ADD_ITEM("StartDir", PACKTYPE_STRING, string, shortcut->start_dir);
		ADD_ITEM("icon", PACKTYPE_STRING, string, shortcut->icon);
		ADD_ITEM("ShortcutPath", PACKTYPE_STRING, string, shortcut->shortcut_path);
		ADD_ITEM("LaunchOptions", PACKTYPE_STRING, string, shortcut->launch_options);
		ADD_ITEM("IsHidden", PACKTYPE_INT, u32, shortcut->is_hidden);
		ADD_ITEM("AllowDesktopConfig", PACKTYPE_INT, u32, shortcut->allow_desktop_config);
		ADD_ITEM("OpenVR", PACKTYPE_INT, u32, shortcut->openvr);
		ADD_ITEM("Devkit", PACKTYPE_INT, u32, shortcut->devkit);
		ADD_ITEM("DevkitGameID", PACKTYPE_STRING, string, shortcut->devkit_game_id);
		ADD_ITEM("DevkitOverrideAppID", PACKTYPE_INT, u32, shortcut->devkit_override_app_id);
		ADD_ITEM("LatPlayTime", PACKTYPE_INT, u32, shortcut->last_play_time);
		ADD_ITEM("FlatpakAppID", PACKTYPE_STRING, string, shortcut->flatpak_app_id);
    // clang-format on
    VDFNode *tag_node = vdf_new_node();
    tag_node->name = "tags";
    for (size_t i = 0; i < shortcut->tags.size; i++) {
      VDFItem item;
      item.k = calloc(1, sizeof(size_t));
      sprintf(item.k, "%zu", i);
      item.v.type = PACKTYPE_STRING,
      item.v.data.string = shortcut->tags.tags[i];
      vdf_node_add_item(tag_node, item);
    }
    vdf_node_add_child(n, tag_node);
    vdf_node_add_child(node, n);
  }

  return node;
}
