#include "commons/type.h"
#include "model/net.h"
#include "model/path_tree.h"
#include "util/heap.h"
#include "statistic/dijkstra.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAP 4

struct dijkstra{
  Heap *heap;                       //the heap used to store nodes' weight
  bool  *visited;                    //visited[i] will be 1 if node i has been added to path tree
  Set  **parents;                   //parents[i] record node i's parent nodes' indice
  net_size_t cap;                  //the storage size of this dijkstra calculator
};

Dijkstra *dijkstra_create(net_size_t capacity){
  Dijkstra *dij = malloc(sizeof(Dijkstra));
  assert(dij != NULL);
  dij->heap = heap_create_min_heap(capacity);
  dij->visited = calloc(capacity, sizeof(bool));
  assert(dij->visited != NULL);
  dij->parents = malloc(capacity * sizeof(Set *));
  assert(dij->parents != NULL);
  net_size_t i;
  for(i = 0; i < capacity; i++){
    dij->parents[i] = set_create(INITIAL_CAP);
  }
  dij->cap = capacity;
  return dij;
}

net_size_t dijkstra_capacity(Dijkstra *dij){
  return dij->cap;
}

void dijkstra_destroy(Dijkstra *dij){
  net_size_t i;
  for(i = 0; i < dij->cap; i++){
    set_destroy(dij->parents[i]);
  }
  free(dij->parents);
  free(dij->visited);
  heap_destroy(dij->heap);
  free(dij);
}

static void dijkstra_reset(net_size_t cap, Dijkstra *dij){
  heap_reset(dij->heap);
  memset(dij->visited, 0, sizeof(bool)*dij->cap);
  net_size_t i;
  for(i = 0; i < dij->cap; i++){
    set_reset(cap, dij->parents[i]);
  }
}

static inline void add_parent(net_size_t i, net_size_t parent, Dijkstra *dij){
  set_add(parent, dij->parents[i]);
}

static inline void clear_parent(net_size_t i, Dijkstra *dij){
  set_reset(4, dij->parents[i]);
}

static inline void add_heap(net_size_t i, weight_t weight, Dijkstra *dij){
  heap_add(weight, i, dij->heap);
}

//the context for calculation
typedef struct dij_ctx{
  Net       *net;
  Path_tree *tree;
  net_size_t cur_idx;
  weight_t   cur_weight;
  Dijkstra  *dij;
}Dij_ctx;

#define VISITED(i) (dij->visited[(i)])

//update adjacent node's weight and its parent if necessary
static inline int process_adjacent(net_size_t adjacent, void *ctx){
  Dij_ctx *dij_ctx = (Dij_ctx*)ctx;
  net_size_t cur_idx = dij_ctx->cur_idx;
  Path_tree *tree = dij_ctx->tree;
  Net*  net = dij_ctx->net;
  Dijkstra *dij = dij_ctx->dij;

  if(net_get_node_state(adjacent, net) == BROKEN) return 0;
  /*
    calculate each adjacent node's new weight
    update it if it's less than the old weight
  */
  weight_t weight = dij_ctx->cur_weight + net_get_weight(cur_idx, adjacent, net);
  weight_t old_weight = heap_get_weight(adjacent, dij->heap);

  if(VISITED(adjacent)){
    assert(weight >= old_weight);
    if(weight == old_weight){
      //node[adjacent] has already been added into the path tree
      //path<dij_ctx->cur_idx, adjacent> should be added into the path tree
      path_tree_add(cur_idx, adjacent, weight, tree);
    }
    return 0;
  }else{
    //node[adjacent] isn't in the path tree
    //if it's in the heap, check & update it
    //else add it into the heap
    if(heap_query(adjacent, dij->heap)){
      if(weight > old_weight) return 0;
      if(weight < old_weight){
	clear_parent(adjacent, dij);
	heap_add(weight, adjacent, dij->heap);
      }
      add_parent(adjacent, cur_idx, dij);
      return 0;
    }else{
      add_heap(adjacent, weight, dij);
      add_parent(adjacent, cur_idx, dij);
      return 0;
    }
  }
}

//add a path <parent, dij_ctx->cur_idx> in the path tree
static inline int link_parent(net_size_t parent, void *ctx){
  Dij_ctx *dij_ctx = (Dij_ctx *)ctx;
  path_tree_add(parent, dij_ctx->cur_idx, dij_ctx->cur_weight, dij_ctx->tree);
}

/**
 * firset reset the tree and the dij
 * then calculate the path tree according to the net
 */
void dijkstra_calculate_path_tree(net_size_t root, Net *net, Path_tree *tree, Dijkstra *dij, int option){
  //if this is a broken node, return
  if(net_get_node_state(root, net) == BROKEN) return;

  path_tree_reset(net_default_cap(net), tree);
  dijkstra_reset(net_default_cap(net), dij);
  
  add_parent(root, root, dij);  //set the root's parent to itself
  add_heap(root, 0, dij);    //add current node to heap
  
  Dij_ctx dij_ctx = {
    net, 
    tree,
    root,
    0,
    dij
  };
  net_size_t add_node_idx;
  while(heap_size(dij->heap) > 0){
    //there is still node in the heap
    dij_ctx.cur_idx = heap_pop(dij->heap);
    dij_ctx.cur_weight = heap_get_weight(dij_ctx.cur_idx, dij->heap);
    
    if(option & DIJ_PRINT_PROCESS){
      printf("DIJ::add node %d, weight %lf\n", dij_ctx.cur_idx, dij_ctx.cur_weight);
    }
    //add path <parents[cur_idx], cur_idx> to the path tree
    set_foreach(link_parent, &dij_ctx, dij->parents[dij_ctx.cur_idx]);
    dij->visited[dij_ctx.cur_idx] = 1;
    net_foreach_connect(process_adjacent, &dij_ctx, dij_ctx.cur_idx, dij_ctx.net);
    if(option & DIJ_PRINT_PROCESS){
      path_tree_diagnose(dij_ctx.tree);
    }
  }
}
