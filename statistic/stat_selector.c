#include "statistic/stat_selector.h"

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

Selector *stat_get_selector(SELECTOR_KIND kind){
  Selector *sel = malloc(sizeof(Selector));
  if (sel == NULL) return sel;
  
  switch(kind){
  case SELECTOR_NORMAL:
    sel
  }
}
