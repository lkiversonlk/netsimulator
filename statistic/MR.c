#include "statistic/MR.h"
#include "statistic/clusters.h"

#include <stdio.h>

/****************************************************************************
 * MR.c
 * city.of.beijing@gmail.com
 *
 * calculation process:
 * for every node[i] in the net, which:
 *   node[i] belongs to the biggest max connected cluster in this net
 *   m(r,i) means the count of normal nodes whose distance to node[i] is less than [r]
 * M(r) = SUM on i m(r,i) / count of the biggest cluster
 **************************************************************************/

typedef struct mr_ctx{
  Lattice_Net *net;
  Clusters    *clusters;
  double      cumulate;      //current cumulated m(r,i)
  net_size_t cluster_idx;    //the biggest cluster's index in this net
}MR_CTX;

static int node_belongs_to_biggest_cluster(net_size_t i, void *ctx){
  MR_CTX *mr_ctx = (MR_CTX*)ctx;
  
  if(clusters_get_node_cluster(i, mr_ctx->clusters) == mr_ctx->cluster_idx){
    return 1;
  }
  return 0;
}

//node[j] was in the right distance of some node[i], 
//if node[j] is normal, we add 1 to the cumulation
static int mr_one_node(net_size_t j, void *ctx){
  MR_CTX *mr_ctx = (MR_CTX*)ctx;
  //printf("node %d in radius\n", j);
  if(net_get_node_state(j, mr_ctx->net) == NORMAL){
    mr_ctx->cumulate+=1;
    //printf("now cumulate is %4f\n", mr_ctx->cumulate);
  }
  return 0;
}

double MR_normal(double radius, Lattice_Net *net){
  net_size_t size = net_size(net);
  Clusters *clusters = clusters_create(size);
  clusters_calculate(net, clusters);

  net_size_t biggest_cluster;
  net_size_t count_biggest_cluster = clusters_pop_with_idx(&biggest_cluster, clusters);
  if(count_biggest_cluster == -1){
    //printf("cluster calculation failed\n");
    return;
  }
  MR_CTX mr_ctx = {
  net : net,
  clusters : clusters,
  cumulate : 0.0,
  cluster_idx : biggest_cluster
  };
  
  net_size_t i;
  for(i = 0; i < size; i++){
    if(node_belongs_to_biggest_cluster(i, &mr_ctx)){
      //printf("MR.c::calculating node %d:\n", i);
      lattice_foreach_within(mr_one_node, &mr_ctx, i, radius, radius, net);
    }
  }//for
  double ret = mr_ctx.cumulate;
  //printf("CR.c::cumulate is %4f\n", ret);
  //printf("CR.c::biggest cluster is %d\n", count_biggest_cluster);
  ret /= (double)count_biggest_cluster;
  clusters_destroy(clusters);
  return ret;
}

//node[j] was in the right distance of some node[i], ant node [j] must be in the same cluster with node[i]
//if node[j] is normal, we add 1 to the cumulation
static int mr_one_node_within_biggest_cluster(net_size_t j, void *ctx){
  MR_CTX *mr_ctx = (MR_CTX*)ctx;
  //printf("node %d in radius\n", j);
  if(net_get_node_state(j, mr_ctx->net) == NORMAL && node_belongs_to_biggest_cluster(j, mr_ctx)){
    mr_ctx->cumulate+=1;
    //printf("now cumulate is %4f\n", mr_ctx->cumulate);
  }
  return 0;
}

//calculate mr, only contains both node[i]&[j] belongs to the biggest cluster in this net
double MR_normal_within_biggest_cluster(double radius, Lattice_Net *net){
  net_size_t size = net_size(net);
  Clusters *clusters = clusters_create(size);
  clusters_calculate(net, clusters);

  net_size_t biggest_cluster;
  net_size_t count_biggest_cluster = clusters_pop_with_idx(&biggest_cluster, clusters);
  if(count_biggest_cluster == -1){
    //printf("cluster calculation failed\n");
    return;
  }
  MR_CTX mr_ctx = {
  net : net,
  clusters : clusters,
  cumulate : 0.0,
  cluster_idx : biggest_cluster
  };
  
  net_size_t i;
  for(i = 0; i < size; i++){
    if(node_belongs_to_biggest_cluster(i, &mr_ctx)){
      //printf("MR.c::calculating node %d:\n", i);
      lattice_foreach_within(mr_one_node_within_biggest_cluster, &mr_ctx, i, radius, radius, net);
    }
  }//for
  double ret = mr_ctx.cumulate;
  //printf("CR.c::cumulate is %4f\n", ret);
  //printf("CR.c::biggest cluster is %d\n", count_biggest_cluster);
  ret /= (double)count_biggest_cluster;
  clusters_destroy(clusters);
  return ret;
}

