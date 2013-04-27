#include "statistic/stat_selector.h"

struct selector{
  bool (*select)(net_size_t idx, Net *net, struct selector *self);
  void (*destroy)(struct selector *self);
  void *ctx;
};


static bool normal_node(net_size_t i, Net *net, Selector *self){
  if (net_get_node_state(i, net) == NORMAL){
    return TRUE;
  }
  return FALSE;
}

static bool broken_node(net_size_t i, Net *net, Selector *self){
  if (net_get_node_state(i, net) == BROKEN){
    return TRUE;
  }
  return FALSE;
}

static bool cascade_node(net_size_t i, Net *net, Selector *self){
  if (net_get_node_state(i, net) == CASCADE){
    return TRUE;
  }
  return FALSE;
}

static bool all_node(net_size_t i, Net *net, Selector *self){
  return TRUE;
}

void default_destroy(Selector *sel){
  free(sel);
}

Selector *stat_get_selector(SELECTOR_KIND kind){
  Selector *sel = malloc(sizeof(Selector));
  if (sel == NULL) return sel;
  sel->destroy = default_destroy;
  switch(kind){
  case SELECTOR_ALL:
    sel->select = all_node;
    break;
  case SELECTOR_NORMAL:
    sel->select = normal_node;
    break;
  case SELECTOR_BROKEN:
    sel->select = broken_node;
    break;
  case SELECTOR_CASCADE:
    sel->select = cascade_node;
    break;
  default:
    break;
  }
  return sel;
}

bool stat_selector_select(net_size_t i, Net *net, Selector *sel){
  return sel->select(i, net, sel);
}
