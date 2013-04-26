#ifndef LATTICE_GUASSI_CONNECTOR_H
#define LATTICE_GUASSI_CONNECTOR_H

/****************************************************************
 * a connector connects a lattice net work
 * and every edge was give a guassi random [1, epsilon]
 * cautions:
 *       if you create a connector
 *       the net it connects every time will be exactly the same
 *
 ****************************************************************/
#include "operator/connector.h"

Connector *lattice_guassi_connector_create(double epsilon, int seed);

#endif
