#include "operator/connector.h"

inline void connector_connect(Net *net, Connector *conn){
  //clear the net's situation
  net_reset(net_default_cap(net), net);
  conn->connect(net, conn);
}

inline void connector_destroy(Connector *conn){
  conn->destroy(conn);
}

