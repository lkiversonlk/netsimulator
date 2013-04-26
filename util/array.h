#ifndef ARRAY_H
#define ARRAY_H

#include "commons/type.h"

/*********************************************************
 * a quite simple dynamic increment array
 * usage:
 *   Array *arr = array_create(6);
 *   array_add(data, arr);
 *   array_foreach(.., arr);
 *   array_destroy(arr);
 * it's convinient for add and find 
 * not efficient for remove
 * cautions:
 *   this array won't check if there is repeat data
 *
 *********************************************************/

typedef struct array Array;

//create an array with specified capacity
Array *array_create(net_size_t cap);

//add an element to an array
void array_add(net_size_t data, Array *array);

//call oper_on_each on each data of the array, if one of the call returns a non 0 value
//array_foreach returns non 0 value
//otherwise, returns 0
int array_each(oper_on_each oper, void *ctx, Array *array);

//remove the i's elements of the array and returns it.
//returns NULL if array size is zero 
net_size_t array_remove(net_size_t i, Array *array);

//get the i's element of the array
//return NULL if not exists
net_size_t array_get(net_size_t i, Array *array);

//return the size of the array
inline net_size_t array_size(Array *array);

//destroy the array, not the data members.
//you should manually use a foreach to destroy each data
void array_destroy(Array *array);

//reset the array
void array_reset(net_size_t cap, Array *array);
#endif
