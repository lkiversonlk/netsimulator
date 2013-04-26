#ifndef MR_H
#define MR_H

#include "model/lattice_net.h"

double MR_normal(double radius, Lattice_Net *net);

double MR_normal_within_biggest_cluster(double radius, Lattice_Net *net);

#endif
