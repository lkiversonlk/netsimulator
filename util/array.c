#include "util/array.h"
#include "commons/type.h"

#include <assert.h>
#include <stdlib.h>

//the increment of the capacity of the array
/***************************************************************************************************************************
 * change log:
 *        modify the way removing element, now we simply set the 
 *
 ***************************************************************************************************************************/
#define INCREMENT 10

struct array{
	net_size_t size;
	net_size_t cap;
	data_t *data;
};

//initial the array
static void initial(net_size_t cap, Array *arr){
	arr->data = calloc(cap, sizeof(data_t));
	assert(arr->data != NULL);
	arr->cap = cap;
}
	
//add the array's capacity
static void add_capacity(net_size_t cap, Array *arr){
	arr->cap += cap;
	arr->data = realloc(arr->data, sizeof(data_t) * arr->cap);
	assert(arr->data != NULL);
}

Array *array_create(net_size_t cap){
	Array *arr = malloc(sizeof(Array));
	assert(arr != NULL);
	initial(cap, arr);
	arr->size = 0;
	return arr;
}

void array_add(data_t data, Array *array){
	if(array->cap <= array->size){
		add_capacity(INCREMENT, array);
	}
	array->data[array->size++] = data;
}

int array_each(oper_on_each oper, void *ctx, Array *array){
	net_size_t i;
	net_size_t ret = 0;
	for(i = 0; i < array->size; i ++){
		ret += oper(array->data[i], ctx);
	}
	return ret;
}

data_t array_remove(net_size_t i, Array *array){
	if(i >= array->size){
		return DATA_NOT_EXISTED;
	}
	data_t ret = array->data[i];
	for(; i < array->size - 1; i++){
		array->data[i] = array->data[i+1];
	}
	array->size--;
	return ret;
}

data_t array_get(net_size_t i, Array *array){
	if(i >= array->size){
		return DATA_NOT_EXISTED;
	}
	return array->data[i];
}

net_size_t array_size(Array *array){
	return array->size;
}

void array_destroy(Array *array){
  free(array->data);
  free(array);
}

void array_reset(net_size_t cap, Array *array){
  if(cap < array->cap){
    array->cap = cap;
    array->data = realloc(array->data, array->cap * sizeof(net_size_t));
  }
  array->size = 0;
}
