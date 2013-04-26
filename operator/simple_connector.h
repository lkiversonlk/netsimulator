#ifndef SIMPLE_CONNECTOR_H
#define SIMPLE_CONNECTOR_H

/*******************************************************************
 * simple connector 
 * city.of.beijing@gmail.com
 *
 * this connector will connect the net as lattice net, see below
 *
 *    N -- N -- N
 *    |    |    |
 *    N -- N -- N
 *    |    |    |
 *    N -- N -- N
 *
 *******************************************************************/
#include "operator/connector.h"

Connector *simple_connector_create();

#endif
