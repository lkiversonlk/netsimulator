#include "operator/net_constructor.h"

#include <stdio.h>

#define SIZE 1024

Net *net_construct(const char *path){
  FILE *file = fopen(path, "r");
  if(file == NULL) return NULL;
  char line[SIZE];

  //first line,           [nodes_num, nodes_degree]
  if(fgets(line, SIZE, file)){
    int nodes_num = 0;
    int nodes_degree = 0;
    sscanf(line, "%d,%d", &nodes_num, &nodes_degree);
    
    if(nodes_num > 0 && nodes_degree > 0){
      Net *net = net_create(nodes_degree, nodes_num);
      if(net != NULL){
	/**
	 * construct net step by step
	 * if any error happens,
	 * the construct process will stop
	 * and the net will be destroy, and return NULL
	 */
	if(fgets(line, SIZE, file)){
	  int edges_num = -1;
	  sscanf(line, "%d", &edges_num);
	  //printf("net_constructor::constructing net with %d nodes and %d edges\n", nodes_num, edges_num);
	  if(edges_num >= 0){
	    int i = 0;
	    net_size_t k,m;
	    weight_t weight;
	    /**
	     * construct every edge <k,m> with weight
	     */
	    for(i = 0; i < edges_num; i++){
	      if(fgets(line, SIZE, file)){
		sscanf(line, "%d,%d,%lf",&k,&m,&weight);
		//this is only because every id starts from 1
		//which is different from C program
		k = k - 1;
		m = m - 1;
		if(k < 0 || k >= nodes_num || m < 0 || m >= nodes_num) break;
		net_connect(k, m, weight, net);
		//printf("net_construct::line %d, connecting %d, %d, weight %lf\n",i,  k, m, weight);
	      }else{
		break;
	      }
	    }//for
	    if(i == edges_num){
	      //construct ok
	      fclose(file);
	      return net;
	    }
	  }
	  net_destroy(net);
	}
      }
    }
    fclose(file);
    return NULL;
  }
}
