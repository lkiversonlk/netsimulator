#include "model/net.h"
#include "model/node.h"
#include "commons/type.h"
#include "assert.h"

#include <stdlib.h>
#include <stdio.h>

struct net{
  Node **nodes;
  net_size_t size;
  net_size_t default_cap;            //the default node's degree
  net_size_t broken_nodes_count;     //the count of broken nodes
  net_size_t cascade_nodes_count;    //the count of cascade nodes
};

Net *net_create(net_size_t degree, net_size_t size){
  Net *net = malloc(sizeof(Net));
  assert(net != NULL);
  net->nodes = malloc(size * sizeof(Node*));
  assert(net->nodes != NULL);
  net->default_cap = degree;
  net->size = size;
  net_size_t i;
  for(i = 0; i < size; i++){
    net->nodes[i] = node_create(i, degree, net);
  }
  net->broken_nodes_count = 0;
  net->cascade_nodes_count = 0;
  return net;
}

Node *net_get(net_size_t i, Net *net){
  if(i >= net->size){
    return NULL;
  }
  return net->nodes[i];
}

int net_connect(net_size_t i, net_size_t j, weight_t weight, Net *net){
  Node *ni = net_get(i, net);
  Node *nj = net_get(j, net);
  assert(ni != NULL);
  assert(nj != NULL);
  if(node_get_state(ni) == BROKEN || node_get_state(nj) == BROKEN){
    //one of the node is broken
    return 0;
  }
  //this will do a double link
  node_connect(j, weight, ni);
  return 1;
}

void net_disconnect(net_size_t i, net_size_t j, Net *net){
  Node *ni = net_get(i, net);
  assert(ni != NULL);
  node_disconnect(j, ni);
}

int net_isconnected(net_size_t i, net_size_t j, Net *net){
  Node *ni = net_get(i, net);
  assert(ni != NULL);
  return node_isconnected(j, ni);
}

int net_foreach(oper_on_each oper, void *ctx, Net *net){
  net_size_t i = 0;
  int ret = 0;
  for(; i < net->size; i++){
    ret += oper(i, ctx);
  }
  return ret;
}

int net_foreach_ptr(oper_on_each_ptr oper, void *ctx, Net *net){
  net_size_t i = 0;
  int ret = 0;
  for(; i < net->size; i++){
    ret += oper(net->nodes[i], ctx);
  }
  return ret;
}

int net_foreach_connect(oper_on_each oper, void *ctx, net_size_t i, Net *net){
  Node *node = net_get(i, net);
  assert(node != NULL);
  return node_foreach(oper, ctx, node);
}

weight_t net_get_weight(net_size_t i, net_size_t j, Net *net){
  Node *ni = net_get(i, net);
  assert(ni != NULL);
  return node_get_weight(j, ni);
}

struct break_ctx{
  Net *net;
  Node *node;
  net_size_t i;
};

static int disconnect(net_size_t idx, void *ctx){
  struct break_ctx *b_ctx = ctx;
  node_disconnect(idx, b_ctx->node);
  Node *node = net_get(idx, b_ctx->net);
  assert(node != NULL);
  node_disconnect(b_ctx->i, node);
  return 0;
}

int net_break_node(net_size_t i, Net *net){
  Node *ni = net_get(i, net);
  assert(ni != NULL);
  struct break_ctx b_ctx = {net, ni, i};
  int ret = node_foreach(disconnect, &b_ctx, ni);
  node_set_state(BROKEN, ni);
  net->broken_nodes_count++;
  return ret;
}

void net_break_node_cascade(net_size_t i, Net *net){
  Node *ni = net_get(i, net);
  assert(ni != NULL);
  struct break_ctx b_ctx = {net, ni, i};
  node_foreach(disconnect, &b_ctx, ni);
  node_set_state(CASCADE, ni);
  net->cascade_nodes_count++;
}

static int print_connect(net_size_t i, void *ctx){
  printf("[%7d],", i);
  return 0;
}

void net_diagnose(Net *net){
  printf("Net Diagnose::size %d\n", net->size);
  net_size_t i = 0;
  for(; i < net->size; i++){
    printf("Net Diagnose::node [%7d] ", i);
    printf("state ");
    switch(net_get_node_state(i, net)){
    case NORMAL:
      printf("[normal ] ");
      break;
    case BROKEN:
      printf("[broken ] ");
      break;
    case CASCADE:
      printf("[cascade] ");
      break;
    default:
      printf("[unknown] ");
    }
    printf("connects: ");
    net_foreach_connect(print_connect, NULL, i, net);
    printf("\n");
  }
}

void net_destroy(Net *net){
  net_size_t i;
  for(i = 0; i < net->size; i++){
    node_destroy(net->nodes[i]);
  }
  free(net->nodes);
  free(net);
}

static int reset_node(void *node, void *ctx){
  node_reset(*(net_size_t *)ctx, node);
}

void net_reset(net_size_t cap, Net *net){
  net_foreach_ptr(reset_node, &cap, net);
  net->broken_nodes_count = 0;
}

net_size_t net_size(Net *net){
  return net->size;
}

net_size_t net_default_cap(Net *net){
  return net->default_cap;
}

node_state_t net_get_node_state(net_size_t i, Net *net){
  Node *node = net_get(i, net);
  assert(node != NULL);
  return node_get_state(node);
}

net_size_t net_broken_nodes_count(Net *net){
  return net->broken_nodes_count;
}

net_size_t net_normal_nodes_count(Net *net){
  return net->size - net->broken_nodes_count - net->cascade_nodes_count;
}

net_size_t net_cascade_nodes_count(Net *net){
  return net->cascade_nodes_count;
}
