#include "commons/type.h"
#include "util/set.h"
#include "util/array.h"

Set *set_create(net_size_t cap){
  return array_create(cap);
}

net_size_t set_size(Set *set){
  return array_size(set);
}

void set_add(net_size_t idx, Set *set){
  int i;
  net_size_t size = set_size(set);
  for(; i < size; i++){
    if(array_get(i, set) == idx){
      return;
    }
  }
  array_add(idx, set);
}

int set_get(net_size_t idx, Set *set){
  int j;
  net_size_t size = set_size(set);
  for(j = 0; j < size; j++){
    if(array_get(j, set) == idx){
      return j;
    }
  }
  return -1;
}

void set_remove(net_size_t idx, Set *set){
  int pos = set_get(idx, set);
  if(pos == -1) return ;
  array_remove(pos, set);
}

void set_destroy(Set *set){
  array_destroy(set);
}

void set_reset(net_size_t cap, Set *set){
  array_reset(cap, set);
}

int set_foreach(oper_on_each oper, void *ctx, Set *set){
  return array_each(oper, ctx, set);
}
