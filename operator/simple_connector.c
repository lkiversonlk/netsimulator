#include "operator/connector.h"
#include "model/net.h"

#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

static void simple_connector_destroy(Connector *simple){
  free(simple);
}

//connect a simple lattice net
static void simple_connector_connect(Net *net, Connector *conn){
  net_size_t size = net_size(net);
  net_size_t edge = sqrt(size);
  if((edge * edge) != size){
    printf("simple_connector.c::simple connector requires the net's size must be n*n\n");
    exit(0);
  }
  net_size_t i,j;
  for(i = 0; i < edge; i++){
    for(j = 0; j < edge; j++){
      net_size_t cur = edge * i + j;
      if(j != 0) net_connect(cur, cur - 1, 1, net);
      if(j != (edge - 1)) net_connect(cur, cur + 1, 1, net);
      if(i != 0) net_connect(cur, cur - edge, 1, net);
      if(i != (edge - 1)) net_connect(cur, cur + edge, 1, net);
    }
  }
}

Connector *simple_connector_create(){
  Connector *conn = malloc(sizeof(Connector));
  assert(conn != NULL);
  conn->connect = simple_connector_connect;
  conn->destroy = simple_connector_destroy;
  return conn;
}
