#include "shortcuts.h"
#include <stdlib.h>
#define ADD_ITEM(name, t, data_type, d)                                        \
  {                                                                            \
    VFDItem *item = malloc(sizeof(VDFItem));                                   \
    item->k = name;                                                            \
    item->v.type = type;                                                       \
    item->v.data.data_type = d;                                                \
    node_add_item(item)                                                        \
  }
VDFNode *shortcuts_to_vdf(Shortcuts *s) {
  VDFNode *node = new_node();
  node->name = "shortcuts";
  for (size_t i = 0; i < s->len; i++) {

    Shortcut *shortcut = s->buff[i];

    VDFNode *n = new_node();
    n->name = malloc(4);
    sprintf(n->name, "%d", 4);
    {
      VDFItem *item = malloc(sizeof(VDFItem));
      item.k = "appid";
      item.v.type = PACKTYPE_INT;
      item.v.data.u32 = shortcut->appid;
      node_add_item(item);
    }
    {
      VDFItem *item = malloc(sizeof(VDFItem));
      item.k = "AppName";
      item.v.type = PACKTYPE_STRING;
      item.v.data = shortcut->app_name;
      node_add_item(item);
    }
    {
      VDFItem *item = malloc(sizeof(VDFItem));
      item.
    }

    node_add_child(node);
  }
}
Shortcuts *shortcuts_from_vdf(VDFNode *n);
