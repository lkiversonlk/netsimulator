#ifndef RUNNER_H
#define RUNNER_H

#include "commons/type.h"
#include "model/net.h"
#include "statistic/betweenness.h"

/*******************************************************************
 * runner is responsible to calculate the [net]'s capacity, loop the broken/attack until the no more nodes are broken
 * runner class, you should provide a net to it
 * and call runner to calculate the load & capacity of each node
 * the runner will break the nodes whose load exceed their capacity
 * and return the count of nodes broken in each round, if needed, the
 * runner will report borken nodes' index
 *
 ******************************************************************/
typedef struct runner Runner;

#define RUNNER_CREATE_SILENT 0x00
#define RUNNER_CREATE_PRINT_TREE 0x01
#define RUNNER_CREATE_PRINT_LOAD 0x02
#define RUNNER_CREATE_PRINT_DIJ  0x04

//create a runner
//the runner will be initialized as the size of the net,
//and each node's capacity initialized as the multiple of alpha and the betweenness of the node
//
//if option & RUNNER_CREATE_PRINT_TREE the runner will print each node's short path tree
//if option & RUNNER_CREATE_PIRNT_LOAD the runner will print each node's capacity
//for option & RUNNER_CREATE_SILENT the runner creation process will reveal nothing
Runner *runner_create(Net *net, double alpha, int option);

//destroy a runner
void runner_destroy(Runner *runner);

//reset the runner according to the net, like a create, but 
//if the net's size stays the same, it'll be faster
void runner_reset(Net *net, double alpha, Runner *runner);

//once the runner has been created, the load of each node is already known
//this [net] should be exactly the same with the [net] when the runner is create
//each node's capacity will be recalculated, compared with its capacity, and break out if the load exceeds capacity
//
//return the number of nodes broken in this round
//the broken nodes' indice will be reported
#define RUNNER_LOOP_SILENT 0
#define RUNNER_LOOP_PRINT_TREE 0x01
#define RUNNER_LOOP_PRINT_LOAD 0x02
#define RUNNER_LOOP_PRINT_BROKEN 0x04
net_size_t runner_loop(Net *net, Runner *runner, int option);

//print each node's capacity
void runner_diagnose(Runner *runner);

BC *runner_getbc(Runner *runner);

#endif
