#include "statistic/stat_on_lattice_net.h"
#include "statistic/stat_distance.h"
#include "util/set.h"
#include "model/lattice_net.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


/**
 * this function calculate net's correlation
 * c_sel select nodes that can be used as the center of the correlation calculation
 * e_sel select nodes that can be used as the edge of the correlation calculation
 * value_sel select nodes that their state will be 1
 * type indicate the way to calculate distance between center and edge nodes
 * max, min are the range parameter
 * if define STAT_DEBUG, this will report the calculation detail
 */
double stat_correlation(Selector *c_sel, Selector *e_sel, Selector *value_sel, Distance *distance_cal, double max, double min, Net *net){
   net_size_t size = net_size(net);
   
   net_size_t idx;
   net_size_t count = 0;
   for (idx = 0; idx < size; idx ++) {
      if (stat_selector_select(idx, net, value_sel) == TRUE) {
#ifdef STAT_DEBUG
         printf("stat_correlation::node %d, state 1", idx);
#endif
         count ++;
      }
   }
   double rate = ((double)count)/size;
#ifdef STAT_DEBUG
   printf("stat_correlation::net's rate is %f\n", broken_rate);
#endif
   
   //correlation = SUMij{(Xi - Xaver)(Xj-Xaver)}/(sigma * SUMi{COUNTj})
   //SUMij{(Xi - Xaver)(Xj - Xaver)}
   double dividor = 0;
   //SUMi{COUNTj}
   double division = 0;
   //sigma
   double sigma = 0;
   
   net_size_t i, j;
   for(i = 0; i < size; i++){
      if(stat_selector_select(i, net, c_sel) == FALSE) continue;
      
#ifdef STAT_DEBUG
      printf("stat_correlation::center node %d\n", i);
#endif
      
      //center node's state value
      double state = -broken_rate;
      if(stat_selector_select(i, net, value_sel) == TRUE){
         state += 1;
      }
      sigma += (state * state);
      
      double sub_dividor = 0;
      for(j = 0; j < size; j++){
         if(stat_selector_select(j, net, e_sel) == FALSE) continue;
         
#ifdef STAT_DEBUG
         printf("stat_correlation::edge node %d\n", j);
#endif
         
         double distance = distance_between(i, j, net, distance_cal);
         
         if((distance <= min) || (distance > max)) continue;
         
#ifdef STAT_DEBUG
         printf("stat_correlation::distance is %f\n", distance);
#endif
         
         double sub_state = -broken_rate;
         if(stat_selector_select(j, net, value_sel) == TRUE){
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

/**
 * this function calculate net's correlation
 * c_sel select nodes that can be used as the center of the correlation calculation
 * e_sel select nodes that can be used as the edge of the correlation calculation
 * type indicate the way to calculate distance between center and edge nodes
 * max, min are the range parameter
 * if define STAT_DEBUG, this will report the calculation detail
 */
double stat_count(Selector *c_sel, Selector *e_sel, Distance *distance_cal, double max, double min, Net *net){
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
      if(stat_selector_select(i, net, c_sel) == FALSE) continue;
      
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
         if(stat_selector_select(j, net, e_sel) == FALSE) continue;
         
#ifdef STAT_DEBUG
         printf("stat_correlation::edge node %d\n", j);
#endif
         
         double distance = distance_between(i, j, net, distance_cal);
         
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

/**
 * this function calculate net's correlation
 * c_sel select nodes that can be used as the center of the correlation calculation
 * e_sel select nodes that can be used as the edge of the correlation calculation
 * type indicate the way to calculate distance between center and edge nodes
 * max, min are the range parameter
 * if define STAT_DEBUG, this will report the calculation detail
 */
double stat_distance(Selector *c_sel, Selector *e_sel, Distance *distance_cal, double max, double min, Net *net){
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
      if(stat_selector_select(i, net, c_sel) == FALSE) continue;
      
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
         if(stat_selector_select(j, net, e_sel) == FALSE) continue;
         
#ifdef STAT_DEBUG
         printf("stat_correlation::edge node %d\n", j);
#endif
         
         double distance = distance_between(i, j, net, distance_cal);
         
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

