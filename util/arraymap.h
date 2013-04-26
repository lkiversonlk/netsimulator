#ifndef ARRAY_MAP_H
#define ARRAY_MAP_H

#include "commons/type.h"

/********************************************************************
 * a simple map, used for the net use node's index as key
 * not efficient for remove
 * usage:
 *      Smap *map = smap_create(8);
 *      smap_put(28, data);
 *      data = smap_get(28);
 *      smap_destroy(map);
 ********************************************************************/

typedef struct array_map Smap;

Smap *smap_create(net_size_t cap);
void smap_put(net_size_t key, void *value, Smap *map);
void *smap_get(net_size_t key, Smap *map);
int smap_foreach(oper_on_each oper, void *ctx, Smap *map);
#endif
