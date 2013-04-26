#ifndef DIJKSTRA_H
#define DIJKSTRA_H

/******************************************************************************************************
 * use dijkstra to calculate a net's shortest path tree from specifed node
 * usage:
 *      Dijkstra *dij = dijkstra_create(9);
 *      dijkstra_calcuate_path_tree(3, net, tree, dij);
 ******************************************************************************************************/
#include "model/net.h"
#include "model/path_tree.h"

typedef struct dijkstra Dijkstra;

#define DIJ_PRINT_PROCESS 0x01

//create a dijkstra calculator, the size is the maximum size of the net it can calculated
//initialize the:
//         min_heap
//         visited_flag
//         parents
//         cap
Dijkstra *dijkstra_create(net_size_t size);

//reset the dij's state for next calculation
//void dijkstra_reset(net_size_t cap, Dijkstra *dij);

//return the calculation capacity of this calculator
net_size_t dijkstra_capacity(Dijkstra *dij);

//the function will calculate the shortest path start from <root> to other 
//nodes of the <net>, the result will be stored in the <tree>
void dijkstra_calculate_path_tree(net_size_t root, Net *net, Path_tree *tree, Dijkstra *dij, int option);

//destroy a calculator
void dijkstra_destroy(Dijkstra *dij);

//reset the dijstra between calculation
void dijstra_reset(net_size_t cap, Dijkstra *dij);
#endif
