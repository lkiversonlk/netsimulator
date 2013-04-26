#ifndef CORR_ONE_NODE_H
#define CORR_ONE_NODE_H

#include "commons/type.h"

typedef struct net Net;

double get_correlation_from_one_node(net_size_t idx, double max, double min, Net *net);
#endif
