#ifndef LOAD_BREAK_H
#define LOAD_BREAK_H

/*************************************************************************************************
 * network load break runner
 * given 
 ************************************************************************************************/
#include "commons/type.h"

typedef struct lb_param{
  net_size_t num_n;               //count of nodes
  net_size_t cap;                 //node's capacity

  net_size_t *N;                  //N is the number of nodes of the net
  
  net_size_t epsilon_n;
  double    *epsilon;             //epsilon the edge weight param

  net_size_t alpha_n;
  double    *alpha;               //node capacity param

  net_size_t p_n;
  double    *p;                    //attack param

  net_size_t repeat;              //repeate time
}Lb_param;

void load_break_run(Lb_param *param);

#endif
