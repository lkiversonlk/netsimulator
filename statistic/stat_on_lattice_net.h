#ifndef STAT_ON_LATTICE_NET_H
#define STAT_ON_LATTICE_NET_H

#include "commons/type.h"
#include "model/net.h"
#include "model/lattice_net.h"
#include "statistic/stat_selector.h"
#include "statistic/stat_distance.h"

// 2013.4.6 update
// this function was prepared for CR, MR, ... and so on
// the procession goes like below:
//    1.choose the nodes that could be treated as center point
//    2.process the calculation
// so we add a package of configs to run

// 2013.4.24 update
// the select function was replaced by select class
//

double stat_correlation(Selector *center_sel, Selector *edge_sel, Distance *distance_cal, double max, double min, Net *net);

#endif
