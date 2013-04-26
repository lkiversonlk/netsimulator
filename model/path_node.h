#ifndef PATH_NODE_H
#define PATH_NODE_H

/***********************************************************************
 * path_node
 * city.of.beijing@gmail.com
 * 
 * used in the path tree structure to describe the shortest paths
 * every path node constains:
 *     the index of net node it represents
 *     the weight from the node to the root of the path tree
 *     its next hops in the shortest path tree
 *     its parent hops in the shortest path tree
 *
 * cautions:
 *     this H file should not be used by direct user, currently (2012.11.11)
 *     path node is only used in the calculation of the net betweenness (dijkstra module and net_betweenness module)
 *
 ***********************************************************************/
#include "commons/type.h"

typedef struct path_node Path_node;

net_size_t path_node_id(Path_node *this);

//create a path_node with the index of the net node
Path_node *path_node_create(net_size_t cap, net_size_t idx);

//add a path_node(j) to the nexts of the path node
void path_node_add_next(net_size_t j, Path_node *this);

//clear the nexts of the path node
void path_node_clear_next(Path_node *this);

//add a parent_node(j) to the parents of the path node
void path_node_add_parent(net_size_t j, Path_node *this);

//clear the parents of the path node
void path_node_clear_parents(Path_node *this);

//reset a path node, clear all its state variables
//including:
//    next hops, parent hops, weight to root, 
void path_node_reset(net_size_t cap, Path_node *item);

//destroy a path node
void path_node_destroy(Path_node *this);

//this will call oper on each next hop's id
int path_node_each_next(oper_on_each oper, void *ctx, Path_node *this);

//this will call oper on each next hop's path node 
//int path_node_each_nextptr(oper_on_each_ptr oper, void *ctx, Path_node *this);

//this will call oper on each parent hop's id
int path_node_foreach_parent(oper_on_each oper, void *ctx, Path_node *this);

//this will call oper on each parent hop's path node
//int path_node_each_parent_ptr(oper_on_each_ptr oper, void *ctx, Path_node *this);

//return this node's next hops' size
net_size_t path_node_next_size(Path_node *this);

//return the node's parent hops' size
net_size_t path_node_parent_size(Path_node *this);

//return the specified next hop in path node's next hops
net_size_t path_node_get_next(net_size_t order, Path_node *this);

//return the specified parent hop in path node's parent hops
net_size_t path_node_get_parent(net_size_t order, Path_node *this);

//diagnose the path item
void path_node_diagnose(Path_node *this);

weight_t path_node_get_weight(Path_node *this);
void path_node_set_weight(weight_t weight, Path_node *this);
#endif
