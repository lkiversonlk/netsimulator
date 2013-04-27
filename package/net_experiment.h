#ifndef NET_EXPERIMENT_H
#define NET_EXPERIMENT_H

/****************************************************************************
 * a net experiment consist of these steps
 * 1. make sure the size of the net [n]
 * <after_net_size>
 * 2. make sure the topo of the net [epsilon]
 * <after_net_connect>
 * 3. make sure the capacity of each node [alpha]
 * <after_net_capacity>
 * 4. make sure the inital attack number [p]
 * 5. make sure the count of experiments [rep]
 * 6. run the below loop for [rep] times:
 *    (1) attack the net
 *    <after_attack>
 *    (2) loop the net
 *    <after each loop>
 *    (3) if in this loop, there are nodes broken, then back to (2)
 *    (4) this loop end
 *    <after net steady>
 * 5. clear the experiment
 *
 * usage:
 * when using net_experiment
 * you should first add your operaton package into the experiment
 * then use the specified config file to run the experiment
 *
 *********************************************************************/

#include "model/net.h"
#include "statistic/betweenness.h"

typedef struct net_exp_package Net_Exp_Pak;

typedef struct exp_state{
  net_size_t n;
  double epsilon;
  double alpha;
  double p;
  int repeat;
  int steps;
}Exp_State;

typedef int (*net_exp_oper)(Net *net, BC *bc, Exp_State *state, Net_Exp_Pak *ctx);

enum operation{
  OPER_AFTER_SIZE = 0,
  OPER_AFTER_CONNECT,
  OPER_AFTER_CAPACITY,
  OPER_ATTACK,
  OPER_AFTER_ATTACK,
  OPER_AFTER_EACH_LOOP,
  OPER_AFTER_STEADY,
  OPER_DESTROY,
  OPER_COUNT
};

#define NET_EXP_PACKAGE_N 7
struct net_exp_package{
  /*
  net_exp_oper after_net_size;
  net_exp_oper after_net_connect;
  net_exp_oper after_net_capcity;
  net_exp_oper attack;
  net_exp_oper after_attack;
  net_exp_oper after_each_loop;  //this will be called after the net went steady
  net_exp_oper after_net_steady;
  net_exp_oper destroy_package;
  */
  net_exp_oper opers[OPER_COUNT];
  void *ctx;
};


typedef struct net_experiment Net_Exp;

//create a simple net experiment
Net_Exp *net_experiment_create();

/**
 * add operations to these net experiment
 */
int net_experiment_add_package(Net_Exp_Pak *pak, Net_Exp *net_exp);

//run the experiment, according to the config file
int net_experiment_run(const char *config_file, Net_Exp *net_exp);

//destroy the net experiment
void net_experiment_destroy(Net_Exp *net_exp);

#endif
