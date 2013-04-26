#include "util/random.h"

#include <stdlib.h>

#define MAX_SEED 19880324   //seed #!-_-
#define PRE_STORE 324

struct random{
  int orig;                //the original key 
  int seed;                //the current seed;
  int *stored;
  int current;             //the current idx;
  net_size_t cap;
};

Random *random_create(int seed, net_size_t capacity){
  Random *random = malloc(sizeof(Random));
  assert(random != NULL);
  random->stored = malloc(capacity * sizeof(int));
  assert(random->stored != NULL);
  random->cap = capacity;
  random->orig = random->seed = seed;
  //set the current to cap, so next request
  //will generate randoms by seed orig
  random->current = random->cap;                 
  return random;
}

void random_reset(Random *random){
  if(random->seed == (random->orig + 1)){
    //the current stored randoms are calculated by seed, doesn't 
  }
}

int random_next(Random *random){
  
}

