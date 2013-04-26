#include "util/kvset.h"
#include "commons/type.h"

#include <assert.h>
#include <stdlib.h>

//the increment of the capacity of the kvset
#define INCREMENT 10

struct kvset{ 
	net_size_t size;
	net_size_t cap;
	Pair *data;
};

//initial the kvset
static void initial(net_size_t cap, Kvset *set){
	set->data = calloc(cap, sizeof(Pair));
	assert(set->data != NULL);
	set->cap = cap;
}
	
//add the kvset's capacity
static void add_capacity(net_size_t cap, Kvset *set){
	set->cap += cap;
	set->data = realloc(set->data, sizeof(Pair) * set->cap);
	assert(set->data != NULL);
}

Kvset *kvset_create(net_size_t cap){
	Kvset *set = malloc(sizeof(Kvset));
	assert(set != NULL);
	initial(cap, set);
	set->size = 0;
	return set;
}

void kvset_add(net_size_t idx, weight_t weight, Kvset *set){
	net_size_t i;
	for(i = 0; i < set->size; i++){
		if(set->data[i].key == idx){
			set->data[i].value = weight;
			return;
		}
	}
	if(set->cap <= set->size){
		add_capacity(INCREMENT, set);
	}
	Pair pair = {idx, weight};
	set->data[set->size++] = pair;
}

int kvset_foreach_pair(oper_on_each_ptr oper, void *ctx, Kvset *kvset){
	net_size_t i;
	net_size_t ret = 0;
	for(i = 0; i < kvset->size; i ++){
		ret += oper(&kvset->data[i], ctx);
	}
	return ret;
}

weight_t kvset_remove(net_size_t idx, Kvset *set){
	int i;
	net_size_t size = set->size;
	for(i = 0; i < size; i++){
		if(set->data[i].key == idx){
			break;
		}
	}
	if(i >= set->size){
		return DATA_NOT_EXISTED;
	}
	weight_t ret = set->data[i].value;
	for(; i < set->size - 1; i++){
		set->data[i] = set->data[i+1];
	}
	set->size--;
	return ret;
}

weight_t kvset_get(net_size_t idx, Kvset *set){
	int i;
	net_size_t size = set->size;
	for(i = 0; i < size; i++){
		if(set->data[i].key == idx){
			return set->data[i].value;
		}
	}

	return DATA_NOT_EXISTED;
}

net_size_t kvset_size(Kvset *kvset){
	return kvset->size;
}

void kvset_destroy(Kvset *kvset){
	free(kvset->data);
	free(kvset);
}

void kvset_reset(net_size_t cap, Kvset *kvset){
  if(cap < kvset->cap){
    kvset->cap = cap;
    kvset->data = realloc(kvset->data, kvset->cap * sizeof(Pair));
  }
  kvset->size = 0;
}

int kvset_for_each_key(oper_on_each oper, void *ctx, Kvset *kvset){
	net_size_t i;
	net_size_t ret = 0;
	for(i = 0; i < kvset->size; i ++){
		ret += oper(kvset->data[i].key, ctx);
	}
	return ret;
}
