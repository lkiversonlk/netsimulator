#include "statistic/center_nodes_count.h"
#include "statistic/stat_on_lattice_net.h"
#include <math.h>

/**
 * we only want to count nodes whose distance to the center of the net
 * is within the range, and achieved this by modify the distance function,
 * this fake center include condition is just used to make sure the count 
 * will run for only one round
 */

bool only_one_node(net_size_t idx, Net *net, void *ctx){
  if(idx == 0) return TRUE;
  return FALSE;
}

/**
 * the distance between <i, j> will return the node[j]'s distance to the center of the net.
 */
double fake_distance(net_size_t i, net_size_t j, Net *net, void *ctx){
  net_size_t edge = sqrt(net_size(net));
  net_size_t x = j / edge;
  net_size_t y = j - x * edge;

  double xx = (double)x - (double)(edge - 1) / 2.0;
  xx *= xx;
  double yy = (double)y - (double)(edge - 1) / 2.0;
  yy *= yy;

  return sqrt(xx + yy);
}

Stat *make_center_nodes_count_stat(){
  Stat *ret = make_stat(ALL, DISTANCE_DIRECT, ALL, COUNT);
  if(ret == NULL) return ret;

  ret->is_center = only_one_node;
  ret->distance_func = fake_distance;
  return ret;
}

Stat *make_center_broken_nodes_count_stat(){
  Stat *ret = make_stat(ALL, DISTANCE_DIRECT, BROKEN, COUNT);
  if(ret == NULL) return ret;

  ret->is_center = only_one_node;
  ret->distance_func = fake_distance;
  return ret;
}

Stat *make_center_cascade_nodes_count_stat(){
  Stat *ret = make_stat(ATTACK,DISTANCE_WALK,CASCADE,COUNT);

  if(ret == NULL) return ret;

  ret->is_center = only_one_node;
  ret->distance_func = fake_distance;
  return ret;
}
