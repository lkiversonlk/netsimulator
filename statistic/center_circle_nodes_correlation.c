#include "statistic/center_circle_nodes_correlation.h"
#include "statistic/stat_on_lattice_net.h"

#include <math.h>

/**
 * we want to calculate correlations based on nodes whose distance to the center of the lattice net 
 * is within a specified range.
 */

static double range = 2.0;

bool nodes_in_center(net_size_t j, Net *net, void *ctx){
  if(net_get_node_state(j, net) == NORMAL) return FALSE;
  
  net_size_t edge = sqrt(net_size(net));
  net_size_t x = j / edge;
  net_size_t y = j - x * edge;

  double xx = (double)x - (double)(edge - 1) / 2.0;
  xx *= xx;
  double yy = (double)y - (double)(edge - 1) / 2.0;
  yy *= yy;

  double distance = sqrt(xx + yy);
  if(distance <= range){
    return TRUE;
  }
  return FALSE;
}

/**
 * correlation->result   records the average correlation
 * count->division       records the count of center nodes
 */
Stat *make_in_center_correlation_stat(){
  Stat *ret = make_stat(ALL, DISTANCE_DIRECT, ALL, COUNT | CORRELATION);
  
  if(ret == NULL) return ret;

  ret->is_center = nodes_in_center;
  return ret;
}
