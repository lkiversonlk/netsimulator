
#include "util/set.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "model/lattice_net.h"
#include "model/net.h"
#include "statistic/stat_on_lattice_net.h"

bool is_cascade(net_size_t idx, Net *net, void *ctx){
  if (net_get_node_state(idx, net) == CASCADE){
    return TRUE;
  }
  return FALSE;
}

Stat *make_stat_3(){
  Stat *stat = make_stat(ALL, DISTANCE_DIRECT, ALL, CORRELATION);
  stat->is_center = is_cascade;
  return stat;
}
