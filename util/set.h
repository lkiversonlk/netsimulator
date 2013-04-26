#ifndef SET_H
#define SET_H

/**************************************************************************************************
 * a simple set, just used to store the nodes idx
 * in fact, it's just a more complicated array
 * usage:
 *    Set *set = set_create(4);
 *    set_add(4);
 *    set_add(5);
 *    int pos = set_find(4);
 *    if(pos >= 0) ;                 //4 exists in the set
 *    set_reset(2);
 *    set_destroy(set);
 *************************************************************************************************/
#include "util/array.h"

typedef struct array Set;

//create a set with capacity cap
Set *set_create(net_size_t cap);

//return the size of the set
net_size_t set_size(Set *set);

//add a index i to the set
void set_add(net_size_t i, Set *set);

//try to find if i exists in the set
//if exits return the idx in the set
//return -1 if not exists
int set_get(net_size_t i, Set *set);

//remove the node index idx
void set_remove(net_size_t idx, Set *set);

//destroy the set
void set_destroy(Set *set);

//reset the set, clear it's data
void set_reset(net_size_t cap, Set *set);

//this will call oper on every elments of the set
//return the sum of the returned value
int set_foreach(oper_on_each oper, void *ctx, Set *set);

#endif
