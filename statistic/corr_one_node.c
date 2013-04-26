#include "statistic/corr_one_node.h"
#include "model/lattice_net.h"


double get_correlation_from_one_node(net_size_t idx, double max, double min, Net *net){
  net_size_t broken_nodes_count = net_broken_nodes_count(net);
  net_size_t size = net_size(net);
  double broken_rate = (double)broken_nodes_count / size;

  double dividor = 0;  
  net_size_t count = 0;
  net_size_t i;
  double sigma = 0;
  for(i = 0; i < size; i++){
    double state = -1 * broken_rate;
    if(net_get_node_state(i, net) == BROKEN){
      state += 1;
    }
    sigma += (state * state);
    
    double distance = distance_direct(idx, i, net, NULL);
    if((distance > min) && (distance <= max)){
      dividor += state;
      count += 1;
    }
  }//for
  sigma /= (double)size;
  double mul = -1 * broken_rate;
  if(net_get_node_state(idx, net) == BROKEN) mul += 1;
  dividor *= mul;
  return dividor / count / sigma;
}
