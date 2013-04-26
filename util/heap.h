#ifndef HEAP_H
#define HEAP_H

/*****************************************************************************************
 * a special heap
 * all the data in the heap will be sorted by the weight, and the heap size is allocated
 * as big as the size of the net, it can also be used as a cache structure
 * you can get a node's weight or update it by the index of the node
 * and you can easily query whether a node exists in the heap
 * cautions:
 *         if you want to use the cache function of the heap
 *         please only pop after all the data has been add,
 *         and once you start pop data, adding new data is not allowed
 * usage:
 * 
 *****************************************************************************************/
#include "commons/type.h"

typedef struct heap Heap;

//reset the heap, clear the state in the heap
void heap_reset(Heap *);

//create a heap, you should use the size of the net to initilized it
Heap *heap_create_min_heap(net_size_t size);
Heap *heap_create_max_heap(net_size_t size);

//cautions:
//this function will call oper on all the data that has onced been in the heap!
//including those has been poped out
//this is mainly used as a cache structure to record the indices of nodes that has been calculated
int heap_foreach(oper_on_each oper, void *ctx, Heap *heap);

//add a node i with weight to the heap
//if node is alrealy in the heap, the new weight will replace the old weight
void heap_add(weight_t weight, net_size_t i, Heap *heap);

//print the heap for diagnose
void heap_print(Heap *heap);

//return the data's label in the top of the heap
//if there is no data left, return -1
net_size_t heap_pop(Heap *heap);

//return the number of data in the heap
net_size_t heap_size(Heap *heap);

//destroy a heap
void heap_destroy(Heap *heap);

//return the node i's weight, if the i's weight has been add into the heap
weight_t heap_get_weight(net_size_t i, Heap *heap);

//check if node[i] is in the heap
int heap_query(net_size_t i, Heap *heap);

#endif
