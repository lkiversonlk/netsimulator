#ifndef BETWEENNESS_H
#define BETWEENNESS_H

/************************************************************************************************************************
 * net betweenness
 *
 ************************************************************************************************************************/
#include "model/net.h"
#include "model/path_tree.h"
#include "statistic/dijkstra.h"

#include <gmp.h>

typedef struct bc BC;

//create a net betweenness calculator
//specify the calculation capacity
BC *bc_create(net_size_t size);

//reset all the bc's state except the size
void bc_reset(BC *bc);

//return the size of the calculator
net_size_t bc_size(BC *bc);

//this will calculate each node's betweenness centrality using the dij calculator & the tree & the net
//cautions:
//    the method will reset bc before each calculation
//    all you have to do is passed in correct tree, dij, bc with enough capacity
//if(option & BC_PRINT_TREE) the calculation will print the tree of each root
#define BC_PRINT_TREE (0x01)
#define BC_PRINT_DIJ_PROCESS (0x02)
void bc_calculate(Net *net, Path_tree *tree, Dijkstra *dij, BC *bc, int option);

mpf_t *bc_get(net_size_t i, BC *bc);

void bc_destroy(BC *bc);

void bc_diagnose(BC *bc);

#endif
