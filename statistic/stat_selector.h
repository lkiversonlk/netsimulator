#ifndef STAT_SELECTOR_H
#define STAT_SELECTOR_H

#include "commons/type.h"
#include "model/net.h"

typedef struct selector{
  bool (*select)(net_size_t idx, Net *net, struct selector *self);
  void (*destroy)(struct selector *self);
  void *ctx;
}Selector;

typedef enum selector_kind{
  SELECTOR_NORMAL,
  SELECTOR_BROKEN,
  SELECTOR_CASCADE
}SELECTOR_KIND;

Selector *stat_get_selector(SELECTOR_KIND kind);

bool stat_select(net_size_t i, Net *net, Selector *sel){
  return sel->select(i, net, sel);
}

#endif
