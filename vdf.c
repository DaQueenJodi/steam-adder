#include "vdf.h"
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#define MAX_STR_SIZE 1024
static VDFItemType read_type(uint8_t **buff) {
  VDFItemType result = *buff[0];
  *buff += 1;
  return result;
}

static char *read_str(uint8_t **buff) {
  size_t len = strnlen((char *)*buff, MAX_STR_SIZE);

  char *str = strndup((char *)*buff, len);

  *buff += len + 1; // plus null terminator

  return str;
}

static uint32_t read_int(uint8_t **buff) {
  uint32_t result;
  memcpy(&result, *buff, sizeof(uint32_t));
  *buff += sizeof(uint32_t);
  return result;
}

typedef enum {
  Nothing,
  MakeChildNode,
  DoneWithNode,
} NextStatus;

static int next_shortcut_item(uint8_t **buff, VDFItem *item) {

  VDFItemType type = (VDFItemType) * *buff;
  *buff += 1;

  if (type == PACKTYPE_NONE) {
    return MakeChildNode;
  }
  if (type == PACKTYPE_NULLMARKER) {
    return DoneWithNode;
  }
  item->v.type = type;
  item->k = read_str(buff);

  switch (type) {
  case PACKTYPE_STRING: {
    item->v.data.string = read_str(buff);
    break;
  }
  case PACKTYPE_WSTRING: {
    assert(0 && "not implemented");
  }
  case PACKTYPE_INT: {
    item->v.data.u32 = read_int(buff);
    break;
  }

  default: {
    fprintf(stderr, "unknown packtype: %x\n", type);
    exit(1);
  }
  }
  return Nothing;
}

void fill_node(VDFNode *node, uint8_t **buff) {
  NextStatus status;
  while ((status = next_shortcut_item(buff, &node->items[node->item_count])) !=
         DoneWithNode) {
    if (status == MakeChildNode) {
      // node->child_nodes[node->child_node_count++] = new_node(read_str(buff));
      fill_node(node->child_nodes[node->child_node_count++], buff);
    }
    node->item_count += 1;
  }
}
static void parse_file(VDFNode *node, uint8_t **buff);

VDFNode *vdf_deserialize(char *path) {

  FILE *f = fopen(path, "rb");

  fseek(f, 0, SEEK_END);
  size_t len = ftell(f);
  rewind(f);

  uint8_t *buff = malloc(len);
  uint8_t *base_buff = buff; // save pointer so we can free it later
  if (buff == NULL) {
    perror("failed to allocate vdf file  buffer");
    exit(1);
  }
  if (fread(buff, len, 1, f) == 0) {
    perror("failed to read file");
    exit(1);
  }
  VDFNode *node = new_node();
  parse_file(node, &buff);

  free(base_buff);
  return node;
}

void vdf_print(VDFNode *node) {
  while (node != NULL) {
    printf("%s:\n", node->name);
    for (size_t node_i = 0; node_i < node->child_node_count; node_i++) {
      for (size_t item_i = 0; item_i < node->item_count; item_i++) {
        VDFItem item = node->items[item_i];
        if (item.k == NULL) { // go to child node
          break;
        }
        printf("  %s: ", item.k);
        switch (item.v.type) {
        case PACKTYPE_INT: {
          printf("%u\n", item.v.data.u32);
          break;
        }
        case PACKTYPE_STRING: {
          printf("%s\n", item.v.data.string);
          break;
        }
        default:
          fprintf(stderr, "unexpected packtype: %s\n",
                  vdf_type_str(item.v.type));
          exit(1);
        }
      }
    }
  }
}

char *vdf_type_str(VDFItemType type) {
  // clang-format off
  switch (type) {
  case PACKTYPE_NONE				: return "NONE"				;
  case PACKTYPE_STRING			: return "STRING"			;
  case PACKTYPE_INT					: return "INT"				;
  case PACKTYPE_WSTRING			: return "WSTRING"		;
  case PACKTYPE_NULLMARKER	: return "NULLMARKER"	;
  default:
	  fprintf(stderr, "unknown: %x \n", type);
		exit(1);
  }

  // clang-format on
}

VDFNode *new_node(void) {
  VDFNode *n = malloc(sizeof(VDFNode));
  n->name = NULL;
  // 5 nodes is more than enough, realistically you need like 3
  n->child_nodes = calloc(sizeof(VDFNode *), 5);
  n->child_node_count = 0;
  n->item_count = 0;
  // 25 should be more than enough
  n->items = calloc(sizeof(VDFItem), 25);
  return n;
}
// this function does NOT recurse
static void free_node(VDFNode *n) {}

void vdf_clean(VDFNode *n) {}

static void vdf_node_add_item(VDFNode *n, VDFItem i) {
  // TODO: make this dynamic
  n->items[n->item_count++] = i;
}

static void vdf_node_add_child(VDFNode *parent, VDFNode *child) {
  // TODO: make this dynamic
  parent->child_nodes[parent->child_node_count++] = child;
}

static void parse_file(VDFNode *node, uint8_t **buff) {
  while (true) {
    VDFItemType type = read_type(buff);
    VDFItem item;
    item.v.type = type;
    switch (type) {
    case PACKTYPE_WSTRING: {
      assert(0 && "PACKTYPE_WSTRING not implemented");
    }
    case PACKTYPE_NONE: {
      VDFNode *n = new_node();
      n->name = read_str(buff);
      parse_file(n, buff);
      vdf_node_add_child(node, n);
      break;
    }
    case PACKTYPE_NULLMARKER: { // we're done!
      return;
    }
    case PACKTYPE_INT: {
      item.k = read_str(buff);
      item.v.data.u32 = read_int(buff);
      break;
    }
    case PACKTYPE_STRING: {
      item.k = read_str(buff);
      item.v.data.string = read_str(buff);
      break;
    }
    default: {
      assert(0 && "unreachable");
    }
    }
    vdf_node_add_item(node, item);
  }
}
