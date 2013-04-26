#ifndef CONNECTOR_H
#define CONNECTOR_H

/*************************************************************************
 * connector 
 * city.of.beijing@gmail.com
 * 
 * connector is used to initilize a net
 * every connector must provide a connect function and a self-destroy function
 * cautions:
 *      connector_connect will clear the net's state before calling connector's 
 *      connect function
 *************************************************************************/
#include "model/net.h"
#include "model/node.h"

struct connector;
typedef void (*connect_ptr)(Net *net, struct connector *connector);
typedef void (*destroy_ptr)(struct connector *connector);

typedef struct connector{
  connect_ptr connect;         //the connect function
  destroy_ptr destroy;         //the destroy function
  void *data;                  //private data
}Connector;

// this will reset the net's infomation at first
// then use connect to initilize the net
inline void connector_connect(Net *net, Connector *conn);

// destroy the connector
inline void connector_destroy(Connector *conn);

#endif
