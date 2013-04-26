#include "statistic/CR.h"

#include <stdio.h>

typedef struct CR_CTX{
  Lattice_Net * net;
  double dividor;
  double division;
  double broken_rate;
}CR_CTX;

static int is_node_broken(net_size_t i, void *ctx){
  CR_CTX *cr_ctx = (CR_CTX *)ctx;
  if(net_get_node_state(i, cr_ctx->net) == BROKEN) return 1;
  return 0;
}

//node j was in node<calculate>'s radius range
//add (state[j] - broken_rate) to dividor
//add (1 to division)
static int cr_one_node(net_size_t j, void *ctx){
  CR_CTX *cr_ctx = (CR_CTX*)ctx;
  cr_ctx->dividor -= cr_ctx->broken_rate;
  if(net_get_node_state(j, cr_ctx->net) == BROKEN){
    cr_ctx->dividor += 1;
  }
  cr_ctx->division += 1;
  return 0;
}

double CR(contains contain, double radius, double width, Lattice_Net *net){
  net_size_t i = 0;
  net_size_t size = net_size(net);
  double x_aver = ((double)(net_broken_nodes_count(net)))/size;
  double sigma = 0;
  double dividor = 0;
  double division = 0;
  
  CR_CTX bad_cr = {
     net : net,
     dividor : 0,
     division : 0,
     broken_rate : x_aver
  };
  
  double sigma_add;
  for(; i < size; i++){
    if(net_get_node_state(i, net) == NORMAL){
      //printf("CR.c::node %d is normal\n", i);
      sigma_add = -1 * x_aver;
    }else{
      //printf("CR.c::node %d is broken\n", i);
      sigma_add = 1 - x_aver;
    }
    sigma += (sigma_add * sigma_add);
    if(contain(i, &bad_cr)){
      bad_cr.dividor = 0;
      bad_cr.division = 0;
      lattice_foreach_within(cr_one_node, &bad_cr, i, radius, width, net);
      dividor += (sigma_add * bad_cr.dividor);
      division += bad_cr.division;
    }
  }//for
  
  sigma /= (double)size;
  //printf("CR.c::sigma is %4f, dividor is %4f, division is %4f\n", sigma, dividor, division); 
  double ret = dividor / division / sigma;
  return ret;
}

double CR_broken(double radius, double width, Net *net){
  return CR(is_node_broken, radius, width, net);
}

