#ifndef STAT_SELECTOR_H
#define STAT_SELECTOR_H

#include "commons/type.h"
#include "model/net.h"

typedef struct selector Selector;

#define  SELECTOR_ALL 0x01
#define  SELECTOR_NORMAL 0x02
#define  SELECTOR_BROKEN 0x04
#define  SELECTOR_CASCADE 0x08

Selector *stat_get_selector(int type);

bool stat_selector_select(net_size_t i, Net *net, Selector *sel);

void stat_selector_destroy(Selector *sel);

#endif
