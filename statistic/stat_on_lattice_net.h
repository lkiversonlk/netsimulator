#ifndef STAT_ON_LATTICE_NET_H
#define STAT_ON_LATTICE_NET_H

#include "commons/type.h"
#include "model/net.h"
#include "model/lattice_net.h"
#include "statistic/stat_selector.h"

// 2013.4.6 update
// this function was prepared for CR, MR, ... and so on
// the procession goes like below:
//    1.choose the nodes that could be treated as center point
//    2.process the calculation
// so we add a package of configs to run

// 2013.4.24 update
// the select function was replaced by select class
//

//statistic the correlation of this net
typedef enum distance_type{
  DISTANCE_DIRECT,
  DISTANCE_WALK
}DISTANCE_TYPE;

double stat_correlation(Selector *center_sel, Selector *edge_sel, DISTANCE_TYPE type, double max, double min, Net *net);

#endif
