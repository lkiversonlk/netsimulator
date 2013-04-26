#ifndef STAT_SELECTOR_H
#define STAT_SELECTOR_H

#include "commons/type.h"
#include "model/net.h"

typedef struct selector Selector;

typedef enum selector_kind{
  SELECTOR_NORMAL,
  SELECTOR_BROKEN,
  SELECTOR_CASCADE
}SELECTOR_KIND;

Selector *stat_get_selector(SELECTOR_KIND kind);

bool stat_selector_select(net_size_t i, Net *net, Selector *sel){
  return sel->select(i, net, sel);
}

void stat_selector_destroy(Selector *sel){
  sel->destroy(sel);
}

#endif
