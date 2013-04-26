#include "operator/connector.h"
#include "operator/lattice_guassi_connector.h"

#include <math.h>
#include <assert.h>
#include <stdio.h>

typedef struct data{
  int seed;
  double epsilon;
}Data;

static double get_gaussi_aux(double epsilon){
  int i;
  double x = 0;
  for(i = 0; i < 12; i++){
    x += (double)random()/RAND_MAX;
  }
  x = x - 6;
  return 1 + x * epsilon;
}

static double get_gaussi(double epsilon){
  double x;
  while((x = get_gaussi_aux(epsilon)) <= 0);
  return x;
}

static void connect(Net *net, Connector* conn){
  Data *data = conn->data;
  double epsilon = data->epsilon;
  net_size_t size = net_size(net);
  net_size_t edge = sqrt(size);
  if((edge * edge) != size){
    printf("lattice_guassi_connector.c::lattice net requires the net's size must be n*n\n");
    exit(0);
  }
  net_size_t i, j;
  srand(data->seed);

  for(i = 0; i < edge; i++){
    for(j = 0; j < edge; j++){
      net_size_t cur = edge * i + j;
      

      if(j != (edge - 1)){
	weight_t edge_weight = get_gaussi(epsilon);
	net_connect(cur, cur + 1, edge_weight, net);
	net_connect(cur+1,   cur, edge_weight, net);
      }
      if(i != (edge - 1)){
	weight_t edge_weight = get_gaussi(epsilon);
	net_connect(cur, cur + edge, edge_weight, net);
	net_connect(cur + edge , cur, edge_weight, net);
      }
    }//for j
  }//for i
}

static void destroy(Connector *conn){
  free(conn->data);
  free(conn);
}

Connector *lattice_guassi_connector_create(double epsilon, int seed){
  Data *data = malloc(sizeof(Data));
  assert(data != NULL);
  data->seed = seed;
  data->epsilon = epsilon;
  Connector *conn = malloc(sizeof(Connector));
  conn->data = data;
  conn->connect = connect;
  conn->destroy = destroy;
  return conn;
}

