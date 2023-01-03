#pragma once
#include <stdint.h>
#include <stdio.h>

#define SHORTCUT_NUM_PAIRS 16
#define TAGS_NUM_PAIRS 2

// NOTE: this is not a general purpose VDF binary parser, its only for shortcut
// files but could probably be tweaked a bit to work, I just dont care enough

// from
// https://github.com/ValveSoftware/source-sdk-2013/blob/master/mp/src/public/tier1/kvpacker.h
typedef enum {
  PACKTYPE_NONE = 0,
  PACKTYPE_STRING = 1,
  PACKTYPE_INT = 2,
  PACKTYPE_WSTRING = 5,
  // used to mark the end of a block in the binary format
  PACKTYPE_NULLMARKER = 8,
} VDFItemType;

typedef union {
  char *string;
  uint32_t u32;
  uint64_t u64;
  float f;
  uint8_t color[3];
  void *ptr;

} VDFItemData;

typedef char *VDFKey;

typedef struct {
  VDFItemType type;
  VDFItemData data;
} VDFValue;

typedef struct VDFItem {
  VDFKey k;
  VDFValue v;
} VDFItem;

typedef struct VDFNode {
  VDFKey name;
  VDFItem *items;
  size_t item_count;
  size_t child_node_count;
  struct VDFNode **child_nodes;
} VDFNode;

void vdf_serialize(VDFNode *n, char *path);
char *vdf_type_str(VDFItemType type);
VDFNode *vdf_deserialize(char *path);
void vdf_print_children(VDFNode *n, size_t deepness);

VDFNode *vdf_new_node(void);
void vdf_node_add_child(VDFNode *n, VDFNode *c);
void vdf_node_add_item(VDFNode *n, VDFItem item);
void vdf_free(VDFNode *n);
