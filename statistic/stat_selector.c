#include "statistic/stat_selector.h"
#include "commons/type.h"

struct selector{
  int type;
  void (*destroy)(struct selector *self);
  void *ctx;
};


static bool normal_node(net_size_t i, Net *net){
  if (net_get_node_state(i, net) == NORMAL){
    return TRUE;
  }
  return FALSE;
}

static bool broken_node(net_size_t i, Net *net){
  if (net_get_node_state(i, net) == BROKEN){
    return TRUE;
  }
  return FALSE;
}

static bool cascade_node(net_size_t i, Net *net){
  if (net_get_node_state(i, net) == CASCADE){
    return TRUE;
  }
  return FALSE;
}

static bool all_node(net_size_t i, Net *net){
  return TRUE;
}

void default_destroy(Selector *sel){
  free(sel);
}

Selector *stat_get_selector(int type){
  Selector *sel = malloc(sizeof(Selector));
  if (sel == NULL) return sel;
  sel->destroy = default_destroy;
  sel->type = type;
  return sel;
}

bool stat_selector_select(net_size_t i, Net *net, Selector *sel){
  int type = sel->type;
  bool result = FALSE;
  
  if ( type & SELECTOR_ALL ){
    return TRUE;
  }

  if ( type & SELECTOR_NORMAL ){
    result += normal_node(i, net);
  }

  if ( type & SELECTOR_BROKEN ){
    result += broken_node(i, net);
  }

  if ( type & SELECTOR_CASCADE ){
    result += cascade_node(i, net);
  }

  if ( result > 0 ){
    return TRUE;
  }
  return FALSE;
}
