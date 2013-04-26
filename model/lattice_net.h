#ifndef LATTICE_NET_H
#define LATTICE_NET_H

/*********************************************************************************
 * lattice net, it's nodes was arranged as grids
 *
 * all the operation on net can applied on lattice_net
 * but lattice_net has a neighbour concept
 *********************************************************************************/

#include "commons/type.h"
#include "model/net.h"

typedef Net Lattice_Net;
typedef double (*distance)(net_size_t i, net_size_t j, Net *net, void *ctx);

//run [oper] on all the nodes whose distance to node [i] is in (distance-width, distance]
//return the sum of the results
int lattice_foreach_within(oper_on_each oper, void *ctx, net_size_t i, double distance, double width, Lattice_Net *net);

//return the distance from node[i] to node[j]
double lattice_distance(net_size_t i, net_size_t j, Lattice_Net *net);

//sqrt((x1 - x2)^2 + (y1 - y2)^2)
double distance_direct(net_size_t i, net_size_t j, Net *net, void *ctx);

//| x1 - x2 | + | y1 - y2 |
double distance_walk(net_size_t i, net_size_t j, Net *net, void *ctx);

//node[i]'s distance to the center of the net
double distance_to_center(net_size_t i, Net *net, void *ctx);

















#endif
