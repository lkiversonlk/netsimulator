#include "statistic/stat_distance.h"
#include "model/net.h"
#include "model/lattice_net.h"

#include <math.h>

struct distance{
  DISTANCE_TYPE type;
};

static double distance_direct(net_size_t i, net_size_t j, Net *net){
  net_size_t size = net_size(net);
  double edge = sqrt(size);
  
  net_size_t rowi = i / edge;
  net_size_t coli = i - edge * rowi;

  net_size_t rowj = j / edge;
  net_size_t colj = j - edge * colj;

  double xx = rowi - rowj;
  xx *= xx;
  double yy = colj - coli;
  yy *= yy;
  
  return sqrt(xx + yy);
}

static double distance_walk(net_size_t i, net_size_t j, Net *net){
  net_size_t size = net_size(net);
  double edge = sqrt(size);
  
  net_size_t rowi = i / edge;
  net_size_t coli = i - edge * rowi;

  net_size_t rowj = j / edge;
  net_size_t colj = j - edge * colj;

  return abs(rowi - rowj) + abs(coli - colj);
}

Distance *distance_calculator(DISTANCE_TYPE type){
  Distance *distance = malloc(sizeof(Distance));
  if (distance == NULL) return distance;
  
  distance->type = type;

  return distance;
}

double distance_between(net_size_t i, net_size_t j, Net *net, Distance *cal){
  switch (cal->type){
  case WALK:
    return distance_walk(i, j, net);
  case DIRECT:
    return distance_direct(i, j, net);
  default:
    return -1;
  }
}

double distance_between_opposite(net_size_t i, net_size_t j, Net *net, Distance *cal){
  switch (cal->type){
  case WALK:
    return distance_direct(i, j, net);
  case DIRECT:
    return distance_walk(i, j, net);
  default:
    return -1;
  }
}
