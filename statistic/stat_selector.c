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

void default_destroy(Selector *sel){
  free(sel);
}

Selector *stat_get_selector(SELECTOR_KIND kind){
  Selector *sel = malloc(sizeof(Selector));
  if (sel == NULL) return sel;
  sel->destroy = default_destroy;
  switch(kind){
  case SELECTOR_NORMAL:
    sel->select = normal_node;
    break;
  case SELECTOR_BROKEN:
    sel->selector = broken_node;
    break;
  case SELECTOR_CASCADE:
    sel->selector = cascade_node;
    break;
  default:
    break;
  }
  return sel;
}
