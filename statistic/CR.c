#include "statistic/stat_on_lattice_net.h"
#include "statistic/CR.h"

/*************************************************************************
 * city.of.beijing@gmail.com
 *
 * the calulation of CR:
 * every node can be a center node,
 * 1.initial sigma (0), dividor (0), sigma (0), broken_rate (broken_nodes/size)
 *
 * 2.in each center's calculation
 *   2.1 initial sub_dividor(0)
 *   2.2 for each node in the center's raidus
 *     2.2.1 sub_dividor += node.state - broken_rate
 *   2.3 sub_dividor *= center.state - broken_rate
 *   2.4 sigma += (center.state - broken_rate)^2
 *   2.5 dividor += sub_dividor
 *
 * 3.result = dividor / sigma / size
 *************************************************************************/

typedef struct CR_CTX{
  double dividor;
  double sub_dividor;
  double sigma;
  double broken_rate;
  double min;
  double max;
}CR_CTX;

static int cr_stat_pairs(net_size_t i, net_size_t j, Net *net, void *ctx){
  CR_CTX *cr_ctx = (CR_CTX*)ctx;
  cr_ctx->sub_dividor -= cr_ctx->broken_rate;
  if(net_get_node_state(j, net) == BROKEN) cr_ctx->sub_dividor += 1;
  return 0;
}

static int cr_stat(net_size_t center, Net *net, void *ctx){
  CR_CTX *cr_ctx = (CR_CTX*)ctx;
  cr_ctx->sub_dividor = 0;
  stat_lattice_foreach_within(center, every_node, distance_direct, cr_ctx->min, cr_ctx->max, cr_stat_pairs, net, ctx);
  double state = -1 * cr_ctx->broken_rate;
  if(net_get_node_state(center, net) == BROKEN) state += 1;
  cr_ctx->sub_dividor *= state;
  cr_ctx->sigma += (state * state);
  cr_ctx->dividor += cr_ctx->sub_dividor;
  return 0;
}

double CR(double min, double max, Lattice_Net *net){
  net_size_t size = net_size(net);
  CR_CTX cr_ctx = {
    dividor : 0,
    sigma   : 0,
    broken_rate : ((double)(net_broken_nodes_count(net)))/size,
    min     : min,
    max     : max
  };

  stat_on_lattice_net(every_node, cr_stat, net, &cr_ctx);
  return cr_ctx.dividor / cr_ctx.sigma / size / size;
}
