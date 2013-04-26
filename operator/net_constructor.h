#ifndef NET_CONSTRUCTOR_H
#define NET_CONSTRUCTOR_H

/**************************************************************
 * net constructor 
 * construct a net according to the configure file
 *
 * notice:
 * the format of the configure file should be:
 * <nodes_num>,<nodes_degree>
 * <edges_num>
 * <edge_start>,<edge_end>,<edge_weight>      [loop num edges_num]
 *
 **************************************************************/

#include "model/net.h"

Net *net_construct(const char *path);
#endif
