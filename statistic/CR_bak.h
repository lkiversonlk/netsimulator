#ifndef CR_H
#define CR_H

/**********************************************************************************************
 * 
 **********************************************************************************************/

#include "model/lattice_net.h"

//contains was a function to determin whether a specified node[i] should be calculated in the CR calculation
typedef int (*contains)(net_size_t i, void *ctx);

double CR_broken(double radius, double width, Net *net);

#endif
