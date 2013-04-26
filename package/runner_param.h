#ifndef RUNNER_PARAM_H
#define RUNNER_PARAM_H

#include "commons/type.h"

typedef struct runner_param{
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
}Runner_param;

/**
 * construct a runner param from path file
 */
Runner_param *runner_param_construct(const char *path);

/**
 * destroy a runner param
 */
void runner_param_destroy(Runner_param *param);
#endif
