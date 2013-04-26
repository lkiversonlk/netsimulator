#include "statistic/stat_on_lattice_net.h"
#include "util/set.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


/**
 * this function calculate net's correlation 
 * c_sel select nodes that can be used as the center of the correlation calculation
 * e_sel select nodes that can be used as the edge of the correlation calculation
 * type indicate the way to calculate distance between center and edge nodes
 * max, min are the range parameter
 * if define 
 */
double stat_correlation_cascade(Selector *c_sel, Selector *e_sel, DISTANCE_TYPE type, double max, double min, Net *net){
  net_size_t size = net_size(net);

  net_size_t broken_count = net_broken_nodes_count(net);
  double broken_rate = ((double)broken_count)/size;

  //correlation = SUMij{(Xi - Xaver)(Xj-Xaver)}/(sigma * SUMi{COUNTj})

  //SUMij{(Xi - Xaver)(Xj - Xaver)}
  double dividor = 0;

  //SUMi{COUNTj}
  double division = 0;
  
  //sigma
  double sigma = 0;
  
#ifdef STAT_DEBUG
  printf("stat_correlation::net's broken rate is %f\n", broken_rate);
#endif

  net_size_t i, j;
  for(i = 0; i < size; i++){
    if(stat_select(i, net, c_sel) == FALSE) continue;

#ifdef STAT_DEBUG
    printf("stat_correlation::center node %d\n", i);
#endif
    
    //center node's state value
    double state = -broken_rate;
    if(net_get_node_state(i, net) == CASCADE){
      state += 1;
    }
    sigma += (state * state);

    double sub_dividor = 0;
    for(j = 0; j < size; j++){
      if(stat_select(j, net, e_sel) == FALSE) continue;

#ifdef STAT_DEBUG
      printf("stat_correlation::edge node %d\n", j);
#endif

      double distance = 0;
      switch(type){
      case DISTANCE_DIRECT:
	distance = distance_direct(i, j, net, NULL);
	break;
      case DISTANCE_WALK:
	distance = distance_walk(i, j, net, NULL);
	break;
      default:
	break;
      }

      if((distance <= min) || (distance > max)) continue;

#ifdef STAT_DEBUG
      printf("stat_correlation::distance is %f\n", distance);
#endif

      double sub_state = -broken_rate;
      if(net_get_node_state(j, net) == CASCADE){
	sub_state += 1;
      }
      sub_dividor += sub_state;
      division += 1;
    }//for j
    //SUMj (Xi - Xaver)(Xj - Xaver) 
    dividor += (sub_dividor * state);
  }//for i
  
  //SUMi{(Xi-Xaver)^2}/COUNTi
  sigma /= size;
  
  return (dividor/division/sigma);
}

