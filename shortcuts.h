#pragma once
#include "vdf.h"

typedef struct {
  size_t size;
  char **tags;
} Tags;

typedef struct {
  uint32_t appid;
  char *app_name;
  char *exe;
  char *start_dir;
  char *icon;
  char *shortcut_path;
  char *launch_options;
  // all uint32_ts to match the encoding, in reality theyre just booleans
  uint32_t is_hidden;
  uint32_t allow_desktop_config;
  uint32_t allow_overlay;
  uint32_t openvr;
  uint32_t devkit;
  char *devkit_game_id;
  uint32_t devkit_override_app_id;
  uint32_t last_play_time;
  char *flatpak_app_id;
  Tags tags;
} Shortcut;

typedef struct {
  Shortcut **buff;
  size_t len;
  size_t capacity;
} Shortcuts;

VDFNode *shortcuts_to_vdf(Shortcuts *s);
Shortcuts *shortcuts_from_vdf(VDFNode *n);
