#include "commons/type.h"
#include "model/net.h"
#include "util/heap.h"
#include "model/node.h"
#include "statistic/clusters.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

/****************************************************************************************************
 * cluster counter
 * city.of.beijing@gmail.com
 * try to find the connected clusters in the net
 *
 * use DFS to find a cluster once
 * counted[i] indicate the idx of cluster node [i] belongs to, and node[i] has not been calculated yet if counted[i] is NOT_COUNTED
 * the result of clusters has been stored in the heap <cluster id, cluster count>
 *
 ***************************************************************************************/
#define NOT_COUNTED -1
#define COUNTED     -2      //this mean node is broken, it is counted, but doesn't belong to any cluster

struct clusters{
  net_size_t *counted;                   //label whether this node has been counted
  Heap *heap;                      //the heap used to store the result
  net_size_t clusters;             //the count of clusters
  net_size_t next;              //the index of the first node that has not been counted
  net_size_t size;              //the size of the net the cluster is processing
};

Clusters *clusters_create(net_size_t size){
  Clusters *clusters = malloc(sizeof(Clusters));
  assert(clusters != NULL);
  clusters->counted = NULL;
  clusters->heap = NULL;
  clusters->clusters = clusters->next = clusters->size = 0;
  return clusters;
}

//free all the dynamic allocated data in the Clusters
//reset clusters, next and size to 0
static int clusters_reset(Clusters *clusters){
  if(clusters->counted) free(clusters->counted);
  if(clusters->heap) heap_destroy(clusters->heap);
  clusters->counted = NULL;
  clusters->heap = NULL;
  clusters->clusters = clusters->next = clusters->size = 0;
}

struct clusters_ctx{
  Clusters *clusters;
  Net *net;
  net_size_t size;
};

//a loop , scan one cluster
static int dfs_count(net_size_t idx, void *ctx){
  struct clusters_ctx* clusters_ctx = (struct clusters_ctx*)ctx;
  Clusters *clusters = clusters_ctx->clusters;
  Net *net = clusters_ctx->net;
  
  if(net_get_node_state(idx, net) == BROKEN){
    clusters->counted[idx] = COUNTED;
    return 0;         //ignore broken node
  }
  
  if(clusters->counted[idx] != NOT_COUNTED) return 0;                        //ignore counted node
  //a new node
  clusters_ctx->size++;                                       //add a new node
  //printf("clusters.c::add node %d, the cluster's size is %d\n", idx, clusters_ctx->size);

  clusters->counted[idx] = clusters->clusters;                //this node belongs to the current cluster
  net_foreach_connect(dfs_count, ctx, idx, net);              //add this new node's neighor
  return 0;
}

int clusters_calculate(Net *net, Clusters *clusters){
  net_size_t size = net_size(net);
  clusters_reset(clusters);
  clusters->heap = heap_create_max_heap(size);
  clusters->counted = calloc(size, sizeof(net_size_t));
  net_size_t i;
  for(i = 0; i < size; i++){
    clusters->counted[i] = NOT_COUNTED;
  }
  clusters->size = size;
  struct clusters_ctx clusters_ctx = {clusters, net, 0};
  while(clusters->next < size){
    if(clusters->counted[clusters->next] != NOT_COUNTED){
      clusters->next++;
      continue;
    }
    dfs_count(clusters->next, &clusters_ctx);                    //do a dfs travel
    weight_t size = clusters_ctx.size;
    if(size != 0){
      heap_add(size, clusters->clusters++, clusters->heap);  //add the result to the heap
    }
    clusters_ctx.size = 0;
  }//while
  return 1;                                          //succeed
}

net_size_t clusters_count(Clusters *clusters){
  return heap_size(clusters->heap);
}

weight_t clusters_pop(Clusters *clusters){
  net_size_t idx = heap_pop(clusters->heap);
  if(idx == -1) return DATA_NOT_EXISTED;
  return heap_get_weight(idx, clusters->heap);
}

weight_t clusters_pop_with_idx(net_size_t *idx, Clusters *clusters){
  *idx = heap_pop(clusters->heap);
  if(*idx == -1) return DATA_NOT_EXISTED;
  return heap_get_weight(*idx, clusters->heap);
}

net_size_t clusters_get_node_cluster(net_size_t i, Clusters *clusters){
  if(i < 0 || i >= clusters->size){
    return -1;
  }
  if(clusters->counted[i] == NOT_COUNTED || clusters->counted[i] == COUNTED){
    return -1;
  }
  return clusters->counted[i];
}

void clusters_destroy(Clusters *clusters){
  clusters_reset(clusters);
  free(clusters);
}

net_size_t clusters_node_size(Clusters *clusters){
  return clusters->size;
}

void clusters_print_cumulation(Clusters *clusters){
  net_size_t size = clusters->size;
  net_size_t *cumulation = malloc((size + 1) * sizeof(net_size_t)); //cumulation[i] means the count of clusters whose nodes count if no less than i
  net_size_t i=0;

  for(; i <= size; i++){
    cumulation[i] = 0;
  }

  net_size_t cluster_num;
  while((cluster_num = clusters_pop(clusters)) != DATA_NOT_EXISTED){
    //this clusters' nodes count is clusters_num,
    //so cumulation[i]++ (i <= clusters_num)
    //printf("%4d\n",cluster_num);
    for(i = 1; i <= cluster_num; i++){
      cumulation[i]++;
    }
  }
  for(i = 1; i <= size; i++){
    //printf("%4d, %4d\n", i, cumulation[i]);
    printf("%4d,", cumulation[i]);
  }
  printf("\n");

  free(cumulation);
}
