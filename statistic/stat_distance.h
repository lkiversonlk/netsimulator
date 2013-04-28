#ifndef STAT_DISTANCE_H
#define STAT_DISTANCE_H

#include "commons/type.h"
typedef struct distance Distance;
typedef struct net Net;

typedef enum distance_type{
  WALK,
  DIRECT
}DISTANCE_TYPE;

Distance *distance_calculator(DISTANCE_TYPE type);

double distance_betweeen(net_size_t i, net_size_t j, Net *net, Distance *cal);
double distance_between_opposite(net_size_t i, net_size_t j, Net *net, Distance *cal);

#endif
