#include "model/net.h"
#include "model/lattice_net.h"
#include "package/net_experiment.h"
#include "operator/net_constructor.h"

struct private_ctx{
};

/**
 * add operations you want to do when the net size is fixed
 */
static int after_size(Net *net, BC *bc, Exp_State *state, Net_Exp_Pak *pak){
}

/**
 * add operations when the net is connected
 */
static int after_connect(Net *net, BC *bc, Exp_State *state, Net_Exp_Pak *pak){
}

/**
 * add operations when the nodes' capacity has been calculated
 */
static int after_capacity(Net *net, BC *bc, Exp_State *state, Net_Exp_Pak *pak){
}

/**
 * add operations when the first round of random attack happened
 */
static int after_attack(Net *net, BC *bc, Exp_State *state, Net_Exp_Pak *pak){
}

/**
 * add operations after each loop when there are more nodes broken
 * cautious: this method won't be called after the last loop that no more nodes are broken, please use after_steady instead
 *
 */
static int after_loop(Net *net, BC *bc, Exp_State *state, Net_Exp_Pak *pak){
}

/**
 * add operations after the net went steady
 */
static int after_steady(Net *net, BC *bc, Exp_State *state, Net_Exp_Pak *pak){
}

/**
 * operations to free your defined Net_Exp_Pak struct
 * the net, bc, state are all NULL, so please don't use them
 *
 */
static int after_destroy(Net *net, BC *bc, Exp_State *state, Net_Exp_Pak *pak){
}

/**
 * 
 */
int main(int argc, char **argv){
  if (argc != 2){
    printf("wrong input, please give one contruct file\n");
    exit(0);
  }

  Net_Exp *net_exp = net_experiment_create();
  if (net_exp == NULL){
    printf("error, maybe due to memory failed\n");
    exit(0);
  }

  Net_Exp_Pak *package = malloc(sizeof(Net_Exp_Pak));
  if (package == NULL){
    printf("error, maybe due to memory failed\n");
    exit(0);
  }

  struct private_ctx *context = malloc(sizeof(struct private_ctx));
  if (context == NULL){
    printf("error, maybe due to memory failed\n");
    exit(0);
  }

  package->ctx = context;
  net_experiment_add_package(package, net_exp);
  net_experiment_run(argv[1], net_exp);
  net_experiment_destroy(net_exp);
  return 0;
}


