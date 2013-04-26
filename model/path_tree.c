#include "commons/type.h"
#include "model/path_tree.h"
#include "util/heap.h"
#include "model/path_node.h"

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <gmp.h>

struct path_tree{
  net_size_t  root;                 //the index of the root of the tree
  Path_node **paths;                
  bool       *cache;                //cache[i] means if node i has been added to the path tree
  Heap       *heap;                 //heap[i] used to sort the nodes by the distance from root to node i
  mpf_t      *path_count;           //path_count[i] is the count of shortest paths from root to node i 
  net_size_t  cap;                  //the capacity of the number of nodes the tree is able to calculate
  net_size_t  node_cap;             //each path tree node's childs capacity
};

Path_tree *path_tree_create(net_size_t node_cap, net_size_t size){
  Path_tree *tree = malloc(sizeof(Path_tree));
  assert(tree != NULL);
  tree->cap = size;
  tree->paths = malloc(sizeof(Path_node *) * size);
  assert(tree->paths != NULL);
  tree->path_count = malloc(sizeof(mpf_t) * size);
  assert(tree->path_count != NULL);
  net_size_t i = 0;
  for(; i < size; i++){
    tree->paths[i] = path_node_create(node_cap, i);
    mpf_init_set_ui(tree->path_count[i], 0);
  }
  tree->cache = calloc(size, sizeof(bool));
  tree->heap = heap_create_max_heap(size);
  tree->root = -1;
  tree->node_cap = node_cap;
  return tree;
}

void path_tree_destroy(Path_tree *this){
  heap_destroy(this->heap);
  free(this->cache);
  net_size_t i;
  for(i = 0;i < this->cap; i++){
    path_node_destroy(this->paths[i]);
    mpf_clear(this->path_count[i]);
  }
  free(this->path_count);
  free(this->paths);
  free(this);
}

net_size_t path_tree_get_root(Path_tree *tree){
  return tree->root;
}

//reset path nodes, cache, path_count
static int reset_path_node(net_size_t i, void *ctx){
  Path_tree *tree = (Path_tree *)ctx;
  path_node_reset(tree->node_cap, tree->paths[i]);
  mpf_set_ui(tree->path_count[i], 0);
  tree->cache[i] = 0;
  return 0;
}

//reset tree's root, each path tree node, cache, path_count, heap, 
void path_tree_reset(net_size_t cap, Path_tree *tree){
  tree->root = DATA_NOT_EXISTED;
  tree->node_cap = cap;
  heap_foreach(reset_path_node, tree, tree->heap);
  heap_reset(tree->heap);
}

void path_tree_add(net_size_t i, net_size_t j, weight_t weight, Path_tree *this){
  path_node_set_weight(weight, this->paths[j]);
  heap_add(weight, j, this->heap);

  if(i == j){
    //printf("netshortpath.c::创建根%u\n", start);
    this->root = i;
    mpf_set_ui(this->path_count[i], 1);
  }else{
    //printf("netshortpath.c::连接节点%u->%u\n", start, end);
    path_node_add_next(j, this->paths[i]);
    path_node_add_parent(i, this->paths[j]);
    mpf_add(this->path_count[j], this->path_count[i], this->path_count[j]);
  }
}

void path_tree_diagnose(Path_tree *this){
  net_size_t root = this->root;
  printf("PATH_TREE<root%d>::\n", root);
  net_size_t i = 0;
  for(; i < this->cap; i++){
    path_node_diagnose(this->paths[i]);
    gmp_printf("%.0Ff,\n", this->path_count[i]);
  }
  printf("\n");
}

Path_node *path_tree_pop_path(Path_tree *this){
  net_size_t i = heap_pop(this->heap);
  if(i == -1) return NULL;
  return this->paths[i];
}

net_size_t path_tree_pop(Path_tree *this){
  return heap_pop(this->heap);
}

mpf_t *path_tree_get_count(net_size_t i, Path_tree *this){
  if(i < 0 || i >= this->cap){
    return NULL;
  }
  return &this->path_count[i];
}
