#include "model/node.h"
#include "model/net.h"
#include "util/kvset.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct node{
	net_size_t idx;			//the index of this node
	net_size_t degree;		//the out degree
	Net       *net;			//the net
	enum node_state_t state;	//the state of the nodes;
	Kvset     *connects;		//the connected nodes' information
};

Node *node_create(net_size_t idx, net_size_t degree, Net *net){
	Node *node = malloc(sizeof(Node));
	assert(node != NULL);
	node->connects = kvset_create(degree);
	assert(node->connects != NULL);
	node->idx = idx;
	node->degree = 0;
	node->state = NORMAL;
	node->net = net;
	return node;
}

void node_connect(net_size_t idx, weight_t weight, Node *this){
	kvset_add(idx, weight, this->connects);
}

void node_disconnect(net_size_t idx, Node *this){
	kvset_remove(idx, this->connects);
}

int node_isconnected(net_size_t idx, Node *this){
	if(kvset_get(idx, this->connects) != DATA_NOT_EXISTED) return 1;
	return 0;
}

int node_foreach(oper_on_each oper, void *ctx, Node *this){
	return kvset_for_each_key(oper, ctx, this->connects);
}

void node_destroy(Node *this){
	kvset_destroy(this->connects);
	free(this);
}

weight_t node_get_weight(net_size_t idx, Node *this){
	return kvset_get(idx, this->connects);
}

Net *node_get_net(Node *this){
	return this->net;
}

static int print_pair(void *ptr, void *ctx){
  Pair *pair = ptr;
  printf("<%3d - %8f> ", pair->key, pair->value);
}
//unfinished 
void node_diagnose(Node *this){
  printf("NODE[%d]:", this->idx);
  kvset_foreach_pair(print_pair, NULL, this->connects);
  printf("\n");
}

node_state_t node_get_state(Node *this){
  return this->state;
}

node_state_t node_set_state(node_state_t state, Node *this){
  node_state_t ret = this->state;
  this->state = state;
  return ret;
}

void node_reset(net_size_t cap, Node *node){
  kvset_reset(cap, node->connects);
  node->state = NORMAL;
}
