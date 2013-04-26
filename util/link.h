#ifndef LINK_H
#define LINK_H

typedef struct link_node Link_Node;

typedef Link_Node Link;

typedef int (*link_oper)(void *data, void *ctx);

//create an empty link
Link *link_create();

//add a new data to link
//return 1 on success
//return 0 on failure
int link_add_data(void *data, Link *link);

//run oper on each link data
//return the sum of the oper result
int link_foreach(link_oper oper, void *ctx, Link *link);

//destroy a link
//cautions: this doesn't destroy the data
void link_destroy(Link *link);

#endif
