#ifndef NET_H
#define NET_H

/************************************************************
 * city.of.beijing@gmail.com
 * an abstract of net
 * usage:
 *    Net *net = net_create(4, 100);      //for a simple 10*10 lattice net
 *    Node *node_5 = net_get(5, net);
 *
 *    net_connect(4, 6, net); 
 *    if(net_isconnected(4, 6, net)){
 *       do something...;
 *    }
 *    
 *    net_reset(4, net);                     //clear the net's state
 *
 ************************************************************/
#include "commons/type.h"
#include "model/node.h"

typedef struct net Net;

//create a Net with [size] nodes, the size of the net is [size]
//initial all the nodes with [degree], and set its value to default_cap
//initial net's broken_nodes_count to [0]
Net *net_create(net_size_t degree, net_size_t size); 

//get the node index i
//return NULL on error
Node *net_get(net_size_t i, Net *net);

//connect the node i to node j with weight weight
//if one of the node is broken, return 0
//return 1 on success
int net_connect(net_size_t i, net_size_t j, weight_t weight, Net *net);

//disconnect the edge from i to j
void net_disconnect(net_size_t i, net_size_t j, Net *net);

//check if there is edge from i to j
//return 1 if there is, 0 if not
int net_isconnected(net_size_t i, net_size_t j, Net *net);

//call oper on every node of the net
//return the sum of the ret value of these calls
int net_foreach(oper_on_each oper, void *ctx, Net *net);
// this will call oper on every Node* of the net
int net_foreach_ptr(oper_on_each_ptr oper, void *ctx, Net *net);

//these two call oper on node i's adjacent nodes
int net_foreach_connect(oper_on_each oper, void *ctx, net_size_t i, Net *net);

//return the weight of the edge from i to j
//return DATA_NOT_EXISTED if there is no edge <i,j>
weight_t net_get_weight(net_size_t i, net_size_t j, Net *net);

//break the specified node, disconnect it from all its neighbors and label the node's state to break
//add the broken_nodes_count of the net by 1
int net_break_node(net_size_t idx, Net *net);

//print the net information 
void net_diagnose(Net *net);

//destroy the net
void net_destroy(Net *net);

//reset the net's state, 
//reset all the nodes with the [cap]
//reset the net's broken_nodes_count to [0]
void net_reset(net_size_t cap, Net *net);

//return the size of the net
net_size_t net_size(Net *net);

//return the default capacity when building the net
net_size_t net_default_cap(Net *net);

//get the node i's state
node_state_t net_get_node_state(net_size_t i, Net *net);

//return the count of broken nodes in the net
net_size_t net_broken_nodes_count(Net *net);

//return the count of cascade nodes
net_size_t net_cascade_nodes_count(Net *net);

//return the count of good nodes in the net
net_size_t net_normal_nodes_count(Net *net);

//break node and set node's state to CASCADE
void net_break_node_cascade(net_size_t i, Net *net);
#endif
