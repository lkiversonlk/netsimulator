#include "util/link.h"

#include <stdlib.h>

struct link_node{
  void *data;
  struct link_node *next;
};

Link *link_create(){
  Link *ret = malloc(sizeof(Link));
  if(ret == NULL) return ret;
  ret->data = NULL;
  ret->next = NULL;
  return ret;
}

int link_add_data(void *data, Link *link){
  if(link == NULL) return 0;
  while(link->data != NULL && link->next != NULL){
    link = link->next;
  }
  if(link->next == NULL){
    link->next = link_create();
    if(link->next == NULL) return 0;
    link = link->next;
  }
  link->data = data;
  return 1;
}

int link_foreach(link_oper oper, void *ctx, Link *link){
  int ret = 0;
  while(link != NULL && link->data != NULL){
    if(link->data != NULL){
      ret += oper(link->data, ctx);
    }
    link = link->next;
  }
  return ret;
}
 
void link_destroy(Link *link){
  Link *prev;
  while(link != NULL){
    prev = link;
    link = link->next;
    free(prev);
  }
}
