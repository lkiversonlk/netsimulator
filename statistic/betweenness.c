#include "commons/type.h"
#include "statistic/betweenness.h"

#include <assert.h>
#include <stdlib.h>

struct bc{
  mpf_t *loads;
  mpf_t *temp;
  net_size_t size;
};

//static mpf_t mpf_1_s;

//reset the temp result bc
static void reset_temp(BC *bc){
  net_size_t i;
  for(i = 0; i < bc->size; i++){
    mpf_set_ui(bc->temp[i], 0);
  }
}

BC *bc_create(net_size_t size){
  //initialize the static variable mpf 1
  //mpf_init_set_ui(mpf_1_s, 1);
  BC *bc = malloc(sizeof(BC));
  assert(bc != NULL);
  bc->loads = calloc(size, sizeof(mpf_t));
  bc->temp  = calloc(size, sizeof(mpf_t));
  assert(bc->loads != NULL && bc->temp != NULL);
  net_size_t i;
  for(i = 0; i < size; i++){
    mpf_init_set_ui(bc->loads[i], 0);
    mpf_init_set_ui(bc->temp[i], 0);
  }
  bc->size = size;
  return bc;
}

void bc_reset(BC *bc){
  net_size_t i;
  for(i = 0; i < bc->size; i++){
    mpf_set_ui(bc->loads[i], 0);
    mpf_set_ui(bc->temp[i], 0);
  }
}

net_size_t bc_size(BC *bc){
  return bc->size;
}

struct bc_ctx{
  BC *bc;
  net_size_t child;
  Path_tree *tree;
};

static int add_bc_to_parents(net_size_t parent, void *ctx){
  struct bc_ctx *bc_ctx = (struct bc_ctx*)ctx;
  mpf_t temp;
  mpf_init_set_ui(temp, 1);
  mpf_add(temp, temp, bc_ctx->bc->temp[bc_ctx->child]);
  mpf_mul(temp, temp, *path_tree_get_count(parent, bc_ctx->tree));
  mpf_div(temp, temp, *path_tree_get_count(bc_ctx->child, bc_ctx->tree));
  mpf_add(bc_ctx->bc->temp[parent], bc_ctx->bc->temp[parent], temp);
  mpf_clear(temp);
  //load[parent] += count[parent]/count[child] * (1 + load[child])
}

//calculate each nodes betweenness centrality according to this net
static void calculate_temp(Path_tree *tree, BC *bc){
  reset_temp(bc);  //clear the temp result
  net_size_t i;
  Path_node *path;
  struct bc_ctx bc_ctx = {bc, -1, tree };
  while((path = path_tree_pop_path(tree))){
    bc_ctx.child = path_node_id(path);
    path_node_foreach_parent(add_bc_to_parents, &bc_ctx, path);
  }
}

void bc_calculate(Net *net, Path_tree *tree, Dijkstra *dij, BC *bc, int option){
  bc_reset(bc);
  net_size_t i;
  net_size_t size = net_size(net);
  net_size_t cap = net_default_cap(net);

  for(i = 0; i < size; i++){
    if(net_get_node_state(i, net) != BROKEN){
      int dij_opt = 0;
      if(option & BC_PRINT_DIJ_PROCESS){
	dij_opt |= DIJ_PRINT_PROCESS;
      }
      dijkstra_calculate_path_tree(i, net, tree, dij, dij_opt);
      if(option & BC_PRINT_TREE){
	path_tree_diagnose(tree);
      }
      calculate_temp(tree, bc);
      mpf_set_ui(bc->temp[i], 0);
      net_size_t j;
      for(j = 0; j < size; j++){
	mpf_add(bc->loads[j], bc->loads[j], bc->temp[j]);
      }
      reset_temp(bc);
    }
  }
}

mpf_t *bc_get(net_size_t i, BC *bc){
  return &bc->loads[i];
}

void bc_destroy(BC *bc){
  net_size_t i;
  for(i = 0; i < bc->size; i++){
    mpf_clear(bc->loads[i]);
    mpf_clear(bc->temp[i]);
  }
  free(bc->loads);
  free(bc->temp);
  free(bc);
}

void bc_diagnose(BC *bc){
  net_size_t i;
  for(i = 0; i < bc->size; i++){
    gmp_printf("node %d:: load %Ff\n", i, *bc_get(i, bc));
  }
}
