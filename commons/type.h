#ifndef TYPE_H
#define TYPE_H

#include <stdint.h>
#include <stdlib.h>

typedef int32_t net_size_t;
typedef net_size_t data_t;
typedef int8_t bool;
#define TRUE (1)
#define FALSE (0)
typedef double weight_t;

#define DATA_NOT_EXISTED (-1)
#define RAND(n) (rand() % (n))

typedef int (*oper_on_each)(net_size_t i, void *ctx);
typedef int (*oper_on_each_ptr)(void *, void *);

typedef enum node_state_t{
  NORMAL,
  BROKEN,
  CASCADE,
  DISCONNECTED
}node_state_t;

#endif
