#ifndef CLUSTERS_H
#define CLUSTERS_H

/************************************************************************
 * max clustersec 
 * calculate the maximum clustersected cluster's number in the net
 * usage:
 *     Clusters *clusters = clusters_create(5);       //a calculator which can calculate at most 5 clusters
 *     clusters_calculate(net);
 *     while(clusters_count(clusters) > 0){
 *       net_size_t number = clusters_pop(clusters);
 *     }
 *     clusters_destroy(clusters)
 *************************************************************************/
#include "commons/type.h"
#include "model/net.h"

struct clusters;
typedef struct clusters Clusters;

//create a net cluster calculator
Clusters *clusters_create(net_size_t size);

//calculate a net's clustersected clusters, return 1 on success
int clusters_calculate(Net *net, Clusters *clusters);

//return the calculated clusters' count
net_size_t clusters_count(Clusters *clusters);

//return the current biggest cluster's node count (excluding the already popped)
//if there are no more clusters, return DATA_NOT_EXISTED
weight_t clusters_pop(Clusters *clusters);

//return the current biggest cluster's node amount (excluding the already popped)
//also return the index of this cluster in [idx]
//if there are no more clusters left, return DATA_NOT_EXISTED, idx will be [-1]
weight_t clusters_pop_with_idx(net_size_t *idx, Clusters *clusters);

//if node[i] was in cluster [j], return j
//if node[i] doesn't exist in any cluster (i negative or too big, or node[i] is broken), return -1
net_size_t clusters_get_node_cluster(net_size_t i, Clusters *clusters);

//destroy the clusters struct 
void clusters_destroy(Clusters *clusters);

//return the count of nodes in the net which has been calculated
net_size_t clusters_node_size(Clusters *clusters);

void clusters_print_cumulation(Clusters *clusters);

#endif
