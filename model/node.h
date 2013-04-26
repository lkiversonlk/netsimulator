#ifndef NODE_H
#define NODE_H

/************************************************************************
 * the node structure of the net
 *
 ************************************************************************/
#include "commons/type.h"

struct net;
typedef struct node Node;

//create a new node with specified degree, the degree may be an estimated value
Node *node_create(net_size_t index, net_size_t degree, struct net *net);

void node_connect(net_size_t idx, weight_t weight, Node *this);			//连接指定节点

void node_disconnect(net_size_t idx, Node *this);				//断开指定节点

//if this connects to node(idx)
//return 1, otherwise return 0
int node_isconnected(net_size_t idx, Node *this);

//oper will be called on every connected node 
int node_foreach(oper_on_each oper, void *ctx, Node *this);

void node_destory(Node *this);

//get the weight on edge <this, idx2>, 
//return DATA_NOT_EXISTED if this isn't connect to idx2
weight_t node_get_weight(net_size_t idx2, Node *this); 

//return the node's net
struct net *node_get_net(Node *this);						

//print node's infomation for diagnose
void node_diagnose(Node *this);

//return the state of the node
node_state_t node_get_state(Node *this);

// set the state of the node
// return the original state of the node
node_state_t node_set_state(node_state_t state, Node *this);

// reset every node
// set the node's state to NORMAL, clear its connected nodes
// and set the set's capacity to cap if cap is less than the current size of the node's connects set
void node_reset(net_size_t cap, Node *node);
#endif
