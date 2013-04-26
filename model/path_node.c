#include "commons/type.h"
#include "model/path_node.h"
#include "util/set.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

struct path_node{
  net_size_t id;
  Set       *nexts;
  Set       *parents;
  weight_t   weight;
};

net_size_t path_node_id(Path_node *this){
  return this->id;
}

Path_node *path_node_create(net_size_t cap, net_size_t id){
  Path_node *node = malloc(sizeof(Path_node));
  assert(node != NULL);
  node->nexts = set_create(cap);
  node->parents = set_create(cap);
  node->id = id;
  node->weight = 0;
  return node;
}

void path_node_add_next(net_size_t j, Path_node *this){
  set_add(j, this->nexts);
}

void path_node_clear_next(Path_node *this){
  set_reset(set_size(this->nexts), this->nexts);
}

void path_node_add_parent(net_size_t j, Path_node *this){
  set_add(j, this->parents);
}

void path_node_clear_parents(Path_node *this){
  set_reset(set_size(this->parents), this->parents);
}

void path_node_reset(net_size_t cap, Path_node *this){
  set_reset(cap, this->nexts);
  set_reset(cap, this->parents);
  this->weight = 0;
}

void path_node_destroy(Path_node *this){
  set_destroy(this->nexts);
  set_destroy(this->parents);
  free(this);
}

int path_node_foreach_next(oper_on_each oper, void *ctx, Path_node *this){
  return set_foreach(oper, ctx, this->nexts);
}

int path_node_foreach_parent(oper_on_each oper, void *ctx, Path_node *this){
  return set_foreach(oper, ctx, this->parents);
}

net_size_t path_node_next_size(Path_node *this){
  return set_size(this->nexts);
}

net_size_t path_node_parent_size(Path_node *this){
  return set_size(this->parents);
}

net_size_t path_node_get_next(net_size_t order, Path_node *this){
  return set_get(order, this->nexts);
}

net_size_t path_node_get_parent(net_size_t order, Path_node *this){
  return set_get(order, this->parents);
}

static int print(net_size_t i, void *ctx){
  printf("-%u-", i);
}

void path_node_diagnose(Path_node *this){
  printf("%d,", this->id);
  set_foreach(print, NULL, this->parents);
  printf(",");
  set_foreach(print, NULL, this->nexts);
  printf(",");
  printf("%lf,", path_node_get_weight(this));
  printf(",");
}

weight_t path_node_get_weight(Path_node *this){
  return this->weight;
}

void path_node_set_weight(weight_t weight, Path_node *this){
  this->weight = weight;
}
