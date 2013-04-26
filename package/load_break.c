#include "commons/type.h"
#include "model/net.h"
#include "model/path_tree.h"
#include "statistic/dijkstra.h"
#include "statistic/betweenness.h"
#include "statistic/clusters.h"
#include "operator/attack.h"
#include "operator/connector.h"
#include "operator/lattice_guassi_connector.h"
#include "package/load_break.h"
#include <gmp.h>
#include <time.h>
#include <stdio.h>

static mpf_t *capacity = NULL;
static net_size_t cap_n = 0;

void print(){
  net_size_t i;
  for(i = 0; i < cap_n; i++){
    gmp_printf("node %d capacity %Ff\n", i, capacity[i]);
  }
}

// use the load calculated in bc, multiply (1 + alpha) 
// as the capacity of every node
void initial(net_size_t n, double alpha, BC *bc){
  net_size_t i;
  if(cap_n != n){
    for(i = 0; i < cap_n; i++){
      mpf_clear(capacity[i]);
    }
    if(capacity) free(capacity);
    capacity = malloc(sizeof(mpf_t) * n);
    cap_n = n;
    for(i = 0; i < cap_n; i++){
      mpf_init(capacity[i]);
    }

    cap_n = n;
  }
  for(i = 0; i < cap_n; i++){
    mpf_set_d(capacity[i], 1 + alpha);
    mpf_mul(capacity[i], capacity[i], *bc_get(i, bc));
  }
}  

void finalize(){
  net_size_t i;
  for(i = 0; i < cap_n; i++){
    mpf_clear(capacity[i]);
  }
  free(capacity);
  capacity = NULL;
}

struct load_ctx{
  BC *bc;
  Net *net;
};

//for a give node, first check whether it's broken already
//if not, then check whether its load exceeds its capacity
//if it is, break this node, and return 1
//else return 0
static int find_broken(net_size_t idx, void *ctx){
  struct load_ctx *l_ctx = (struct load_ctx*)ctx;
  if(net_get_node_state(idx, l_ctx->net) == NORMAL){
    if(mpf_cmp(*bc_get(idx, l_ctx->bc), capacity[idx]) > 0){
      //the new load exceeds the capacity
      //printf("node[%d] failed\n", idx);
      net_break_node_cascade(idx, l_ctx->net);
      return 1;
    }
  }
  return 0;
}

//loop:
//    check every node's load with its capacity
//    return the count of nodes that are broken
net_size_t loop_broken(Net *net, BC *bc){
  struct load_ctx l_ctx = {bc, net};
  return net_foreach(find_broken, &l_ctx, net);
}

void load_break_run(Lb_param *param){
  printf("   n,     epsi,   alph  ,    p    , r,stp, bigg    ,  sec\n");
  net_size_t i_n, i_ep, i_al, i_p, i_rep;
  for(i_n = 0; i_n < param->num_n; i_n++){
    //printf("load_break.c::for net with %d nodes\n", param->N[i_n]);
    Net *net = net_create(4, param->N[i_n]);
    Path_tree *tree = path_tree_create(4, param->N[i_n]);
    Dijkstra *dij = dijkstra_create(param->N[i_n]);
    BC  *bc = bc_create(param->N[i_n]);
    Clusters *clusters = clusters_create(param->N[i_n]);
    
    for(i_ep = 0; i_ep < param->epsilon_n; i_ep++){
      //for a given epsilon, we can calculate the weight of every edge
      //so we can define the connector
      //printf("load_break.c::for epsilon %f \n", param->epsilon[i_ep]);
      Connector *conn = lattice_guassi_connector_create(param->epsilon[i_ep], time(0));

      for(i_al = 0; i_al < param->alpha_n; i_al++){
	//for a given alpha, we can calculate the net's shortest path tree
	//and we can also initialize the nodes' capacity
	//printf("load_break.c::for alpha %f\n", param->alpha[i_al]);
	connector_connect(net, conn);
	//net_diagnose(net);
	bc_calculate(net, tree, dij, bc,0);	      
	//bc_diagnose(bc);
	initial(param->N[i_n], param->alpha[i_al], bc);
	//print();

	for(i_p = 0; i_p < param->p_n; i_p++){
	  //printf("load_break.c::for p %f \n", param->p[i_p]);

	  int attack_num = ((1-param->p[i_p]) * param->N[i_n] + 0.5);
	  //printf("attack %d\n", attack_num);
	  for(i_rep = 0; i_rep < param->repeat; i_rep++){
	    //printf("load_break.c::repeat %d \n", i_rep);
	    /**
	     * first reconstruct the net
	     * then attack attack_num nodes
	     */
	    connector_connect(net, conn);
	    //printf("load_break.c::construct net\n");
	    struct timeval time;
	    gettimeofday(&time, NULL);
	    //printf("%ld\n", time.tv_usec);
	    attack(attack_num, net, time.tv_usec);
	    bc_calculate(net, tree, dij, bc, 0);
	    //bc_diagnose(bc);
	    net_size_t broken;
	    net_size_t steps = 0;

	    while((broken = loop_broken(net, bc))){
	      steps++;
	      bc_calculate(net, tree, dij, bc,0);
	    }

	    //now we calculate the net's clusters' size 
	    clusters_calculate(net, clusters);
	    weight_t biggest = clusters_pop(clusters);
	    weight_t second  = clusters_pop(clusters);
	    printf("%4d, %4f, %4f, %4f, %d, %d, %4f, %4f\n", param->N[i_n], param->epsilon[i_ep], param->alpha[i_al], param->p[i_p], i_rep, steps, biggest, second);
	  }//i_rep
	}//i_p
      }//i_al
      connector_destroy(conn);
    }//i_ep

    clusters_destroy(clusters);
    bc_destroy(bc);
    dijkstra_destroy(dij);
    path_tree_destroy(tree);
    net_destroy(net);
    finalize();
  }//i_n
}
