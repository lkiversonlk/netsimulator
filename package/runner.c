#include "package/runner.h"
#include "model/path_tree.h"
#include "statistic/dijkstra.h"
#include "statistic/betweenness.h"
#include "statistic/clusters.h"

#include <gmp.h>
#include <stdio.h>
#include <assert.h>

struct runner{
  Path_tree *tree;                    //the root path tree
  Dijkstra *dij;                      //the tree calculator
  BC *bc;                             //betweenness calculator
  mpf_t *capacity;                    //each nodes capacity
  net_size_t size;                    //the size of calculation ability
};

//initialize everything according to the runner except the runner itself
static initial_runner(Net *net, double alpha, Runner *runner, int option){
  net_size_t size = net_size(net);
  net_size_t cap = net_default_cap(net);
  runner->tree = path_tree_create(cap, size);
  assert(runner->tree != NULL);
  runner->dij = dijkstra_create(size);
  assert(runner->dij != NULL);
  runner->bc = bc_create(size);
  assert(runner->bc != NULL);
  runner->capacity = malloc(sizeof(mpf_t)*size);
  assert(runner->capacity != NULL);
  net_size_t i;  
  for(i = 0; i < size; i++){
    mpf_init(runner->capacity[i]);
  }
  runner->size = size;
  
  int bc_opt = 0;
  if(option & RUNNER_CREATE_PRINT_TREE){
    bc_opt |= BC_PRINT_TREE;
  }
  if(option & RUNNER_CREATE_PRINT_DIJ){
    bc_opt |= BC_PRINT_DIJ_PROCESS;
  }
  bc_calculate(net, runner->tree, runner->dij, runner->bc, bc_opt);
  if(option & RUNNER_CREATE_PRINT_LOAD){
    bc_diagnose(runner->bc);
  }

  for(i = 0; i < size; i++){
    mpf_set_d(runner->capacity[i], 1 + alpha);
    mpf_mul(runner->capacity[i], runner->capacity[i], *bc_get(i, runner->bc));
  }
}

//initial a runner according to the net and alpha
Runner *runner_create(Net *net, double alpha, int option){
  Runner *runner = malloc(sizeof(Runner));
  assert(runner != NULL);
  initial_runner(net, alpha, runner, option);
  return runner;
}

//destroy everything except the runner
void destroy_runner(Runner *runner){
  net_size_t size = runner->size;
  net_size_t i;
  for(i = 0; i < size; i++){
    mpf_clear(runner->capacity[i]);
  }
  free(runner->capacity);
  bc_destroy(runner->bc);
  dijkstra_destroy(runner->dij);
  path_tree_destroy(runner->tree);
}

//destroy the runner
void runner_destroy(Runner *runner){
  destroy_runner(runner);
  free(runner);
}

//reset the net acclrding to the net, if the net's size is the same
//with the size of the runner, it only reset the capacity
void runner_reset(Net *net, double alpha, Runner *runner){
  net_size_t size = net_size(net);
  if(size != runner->size){
    destroy_runner(runner);
    initial_runner(net, alpha, runner, 0);
  }else{
    net_size_t i;
    for(i = 0; i < runner->size; i++){
      mpf_set_d(runner->capacity[i], 0);
    }
  }
}

struct load_ctx{
  Runner *runner;
  Net *net;
  int reveal;
};

//for a give node, first check whether it's broken already
//if not, then check whether its load exceeds its capacity
//if it is, break this node, and return 1
//else return 0
static int find_broken(net_size_t idx, void *ctx){
  struct load_ctx *l_ctx = (struct load_ctx*)ctx;
  if(net_get_node_state(idx, l_ctx->net) == NORMAL){
    if(mpf_cmp(*bc_get(idx, l_ctx->runner->bc), l_ctx->runner->capacity[idx]) > 0){
      //the new load exceeds the capacity
      if(l_ctx->reveal){
	printf("node[%d], ", idx);
      }
      net_break_node_cascade(idx, l_ctx->net);
      return 1;
    }
  }
  return 0;
}

//loop:
//    check every node's load with its capacity
//    return the count of nodes that are broken
net_size_t runner_loop(Net *net, Runner *runner, int option){
  int bc_opt = 0;
  if(option & RUNNER_LOOP_PRINT_TREE){
    bc_opt |= BC_PRINT_TREE;
  }
  
  bc_calculate(net, runner->tree, runner->dij, runner->bc, bc_opt);
  if(option & RUNNER_LOOP_PRINT_LOAD){
    bc_diagnose(runner->bc);
  }

  int reveal = (option & RUNNER_LOOP_PRINT_BROKEN);
  struct load_ctx l_ctx = {runner, net, reveal};
  if(reveal){
    printf("new round::");
  }
  net_size_t ret = net_foreach(find_broken, &l_ctx, net);
  if(reveal){
    //broken nodes report, shift a line
    printf("\n");
  }
  return ret;
}

void runner_diagnose(Runner *runner){
  net_size_t i;
  for(i = 0; i < runner->size; i++){
    gmp_printf("%d, %Ff\n", i, runner->capacity[i]);
  }
  //path_tree_diagnose(runner->tree);
}

BC *runner_getbc(Runner *runner){
  return runner->bc;
}
