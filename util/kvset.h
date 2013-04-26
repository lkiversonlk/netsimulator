#ifndef KVSET_H
#define KVSET_H

#include "commons/type.h"

/*********************************************************
 * a quite simple dynamic increment KV set
 * the key is the index of the node
 * the value is the weigtht
 * it's convinient for add and find
 * not efficient for remove
 * usage:
 *     Kvset *set = kvset_create(3);
 *     kvset_add(2, 1.5, set);
 *     kvset_add(3, 0.8, set);
 *     weight_t weight = kvset_get(2, set);
 *     kvset_reset(4, set);                clear the set
 *     kvset_destroy(set);
 *********************************************************/

typedef struct kvset Kvset;

typedef struct pair{
	net_size_t key;
	weight_t   value;
}Pair;

//create an kvset with specified capacity
Kvset *kvset_create(net_size_t cap);

//add an element to an kvset
void kvset_add(net_size_t idx, weight_t weight, Kvset *set);

//call oper_on_each on each Pair* of the kvset, if one of the call returns a non 0 value
//kvset_foreach returns non 0 value
//otherwise, returns 0
int kvset_foreach_pair(oper_on_each_ptr oper, void *ctx, Kvset *kvset);

//remove the element with key i and returns its weight.
//returns DATA_NOT_EXISTED if not found 
weight_t kvset_remove(net_size_t i, Kvset *kvset);

//find the Pair with key i and returns its weight
//return DATA_NOT_EXISTED if not found
weight_t kvset_get(net_size_t i, Kvset *set);

//return the size of the kvset
inline net_size_t kvset_size(Kvset *kvset);

//destroy the kvset, not the data members.
//you should manually use a foreach to destroy each data
void kvset_destroy(Kvset *kvset);

//reset the kvset, clear all the stored data
//set set's capacity to cap, if cap if less than the current capacity of the set
void kvset_reset(net_size_t cap, Kvset *kvset);

//this will call oper on every key, and return the sum of the result
int kvset_for_each_key(oper_on_each oper, void *ctx, Kvset *set);
#endif
