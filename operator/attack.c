#include "operator/attack.h"

#include <stdlib.h>
#include <assert.h>

#define MAX_SEED 1024
#define NET_RAND(n) (rand()%(n))

void attack(net_size_t num, Net *net, int seed){
  //printf("%d\n", seed);
  net_size_t size = net_size(net);
  assert(RAND_MAX > size);
  srand(seed);
  net_size_t random;
  while(num > 0){
    random = NET_RAND(size);
    if(NORMAL == net_get_node_state(random, net)){
      //printf("attack.c::attack node %d\n", random);
      net_break_node(random, net);
      num --;
    }
  }
}
