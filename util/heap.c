#include "util/heap.h"
#include "commons/type.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

typedef int (*compare)(weight_t w1, weight_t w2);

struct heap{
  weight_t *weights;		//weights[i] stores the weight of the nodes index i
  data_t     *heap;		//heap[i], the really heap.
  net_size_t *pos;		//store the position of node i in the heap, so heap[pos[i]] = i
  net_size_t size;		//the size of the heap->heap :: cautions, this is the amount of sorted data
  net_size_t cache_size;		//the size of the used data :: caustions, this is the range of the foreach
  net_size_t cap;			//the capacity of the heap	
  compare cmp; 	
};

#define WEIGHT_NODE(i) (heap->weights[(i)])
#define WEIGHT_POS(i)  (heap->weights[heap->heap[(i)]])
#define HEAP_POS(i)    (heap->pos[(i)])
#define HEAP_VALUE(i)  (heap->heap[(i)])
#define LEFT(i)        (2*(i) + 1)
#define RIGHT(i)       (2*(i) + 2)
#define PARENT(i)      (((i) - 1)>>1)
#define HEAP_SIZE      (heap->size)
#define HEAP_CACHE_SIZE (heap->cache_size)

//reset all the pos elements to heap->cap, which means it doesn't exists in the heap
//reset heap->size, heap->cache_size to 0
void heap_reset(Heap *heap){
  net_size_t i;
  for(i = 0; i < heap->cap; i++){
    HEAP_POS(i) = heap->cap;
    WEIGHT_NODE(i) = DATA_NOT_EXISTED;
  }
  HEAP_SIZE = 0;
  HEAP_CACHE_SIZE = 0;
}

//malloc space for the heap, and set the initial conditions
static void heap_initial(net_size_t cap, Heap *heap){
  heap->weights = malloc(cap * sizeof(weight_t));
  heap->heap    = malloc(cap * sizeof(net_size_t));
  heap->pos     = malloc(cap * sizeof(net_size_t));
  assert(heap->weights != NULL && heap->heap != NULL && heap->pos != NULL);
  heap->cap     = cap;
  heap_reset(heap);
}

//swap two nodes' position in the heap
//the value in the heap, also the position in the pos
static void swap_node(net_size_t i, net_size_t j, Heap *heap){
  HEAP_VALUE(HEAP_POS(i)) = j;
  HEAP_VALUE(HEAP_POS(j)) = i;
  
  net_size_t temp = HEAP_POS(i);
  HEAP_POS(i) = HEAP_POS(j);
  HEAP_POS(j) = temp;
}

static int compare_smaller(weight_t w1, weight_t w2){
  return ((w1 - w2) < 0);
}

static int compare_bigger(weight_t w1, weight_t w2){
  return ((w1 - w2) > 0);
}

//adjust the head structure from pos to childs
//make sure that heap->cmp(root's weight, childs' weight) is always true
static void topdown_adjust(net_size_t pos, Heap *heap){
  net_size_t swap_loc;
  while(pos < HEAP_SIZE){
    swap_loc = LEFT(pos);
    if(swap_loc >= HEAP_SIZE) break;    //no more childs
    if(swap_loc + 1 < HEAP_SIZE){
      //left and right childs
      swap_loc = heap->cmp(WEIGHT_POS(swap_loc), WEIGHT_POS(swap_loc + 1))?swap_loc : swap_loc + 1;
    }
    if(heap->cmp(WEIGHT_POS(pos), WEIGHT_POS(swap_loc))){
      break;
    }
    swap_node(HEAP_VALUE(pos), HEAP_VALUE(swap_loc), heap);
    pos = swap_loc;
  }
}

//buttom up adjust the heap from pos
static void buttomup_adjust(net_size_t pos, Heap *heap){
  if(pos == 0) return;
  net_size_t parent = PARENT(pos);
  while((pos != 0) && (heap->cmp(WEIGHT_POS(pos), WEIGHT_POS(parent)))){
    swap_node(HEAP_VALUE(pos), HEAP_VALUE(parent), heap);
    pos = parent;
    parent = PARENT(pos);
  }
}

static Heap *heap_create(net_size_t size){
  Heap *heap = malloc(sizeof(Heap));
  assert(heap != NULL);
  heap_initial(size, heap);
  return heap;
}

//to check if a node exists in the heap
int heap_query(net_size_t i, Heap *heap){
  return (HEAP_POS(i) < HEAP_SIZE);
}

Heap *heap_create_min_heap(net_size_t size){
  Heap *heap = heap_create(size);
  heap->cmp = compare_smaller;
  return heap;
}

Heap *heap_create_max_heap(net_size_t size){
  Heap *heap = heap_create(size);
  heap->cmp = compare_bigger;
  return heap;
}

int heap_foreach(oper_on_each oper, void *ctx, Heap *heap){
  int ret = 0;
  int i;
  for(i = 0; i < heap->cache_size; i++){
    ret += oper(heap->heap[i], ctx);
  }
  return ret;
}

void heap_add(weight_t weight, net_size_t i, Heap *heap){
  if(heap_query(i, heap)){
    //this node is already in the heap
    if(heap->cmp(weight, WEIGHT_NODE(i))){
      WEIGHT_NODE(i) = weight;
      buttomup_adjust(HEAP_POS(i), heap);
    }else{
      WEIGHT_NODE(i) = weight;
      topdown_adjust(HEAP_POS(i), heap);
    }
  }else{
    //new node
    HEAP_VALUE(HEAP_SIZE) = i;
    HEAP_POS(i) = HEAP_SIZE;
    WEIGHT_NODE(i) = weight;
    heap->size++;
    heap->cache_size++;
    buttomup_adjust(HEAP_SIZE - 1, heap);
  }	
}

void heap_print(Heap *heap){
  net_size_t i = 0;
  for(; i < HEAP_SIZE; i++){
    printf("[%d -- %f]", HEAP_VALUE(i), WEIGHT_POS(i));
  }
  printf("\n");
}

//get the top node's index and put it at the end of the heap cache
//minus the heap size, re adust the heap
net_size_t heap_pop(Heap *heap){
  if(HEAP_SIZE == 0) return -1;
  net_size_t ret = HEAP_VALUE(0);
  HEAP_SIZE = HEAP_SIZE - 1;
  HEAP_VALUE(0) = HEAP_VALUE(HEAP_SIZE);
  HEAP_VALUE(HEAP_SIZE) = ret;
  HEAP_POS(ret) = HEAP_SIZE;
  HEAP_POS(HEAP_VALUE(0)) = 0;
  topdown_adjust(0, heap);
  return ret;
}

net_size_t heap_size(Heap *heap){
  return HEAP_SIZE;
}

void heap_destroy(Heap *heap){
  free(heap->weights);
  free(heap->heap);
  free(heap->pos);
  free(heap);
}

weight_t heap_get_weight(net_size_t i, Heap *heap){
  return WEIGHT_NODE(i);
}
