#ifndef PATH_TREE_H
#define PATH_TREE_H

/*********************************************************************************************************************
 * path tree
 * city.of.beijing@gmail.com
 *
 * this is the tree representation of a net's shortest path tree starting from one root node
 * you may create a tree with a specified capacity.
 * then you can use this tree to represent any network if its node size is less than the capacity
 * before using the tree structure, remember to reset the tree
 * usage:
 *      Path_tree *tree = path_tree_create(cap, size);
 *      path_tree_add(2, 3, 5.2, tree);
 *      path_tree_reset(4, tree);
 *      path_tree_destroy(tree);
 *      
 * cautions:
 *      the tree won't gurantee it is the shortest path tree of net by the tree itself
 *      it is used by other algorithm (like Dijkstra) to achieved that
 *      try to reset the tree and reuse it rather than destroy it and create a new tree
 *
 ********************************************************************************************************************/
#include "model/path_node.h"
#include "util/set.h"

#include <gmp.h>

typedef struct path_tree Path_tree;

//create a path tree, initialize :
//          a max heap
//          a path node index
//          set root to -1
//          set cap to size
Path_tree *path_tree_create(net_size_t node_cap, net_size_t size);

//destroy a path tree
void path_tree_destroy(Path_tree *tree);

//return the root of the new path tree
net_size_t path_tree_get_root(Path_tree *tree);

//reset the path tree, clear its state
void path_tree_reset(net_size_t cap, Path_tree *tree);

//add <i, j, weight> into the tree
void path_tree_add(net_size_t i, net_size_t j, weight_t weight, Path_tree *tree);

void path_tree_diagnose(Path_tree *tree);

//return the path with the biggest weight in the tree, excluding those has been popped
//return NULL if all the paths has been popped
Path_node *path_tree_pop_path(Path_tree *tree);

//return the node with the biggest weight in the tree, excluding those has been popped
//return -1 if all the paths has been popped
net_size_t path_tree_pop(Path_tree *tree);

//return the count of the shortest paths at node i
mpf_t *path_tree_get_count(net_size_t i, Path_tree *tree);

//return the capacity of the tree
net_size_t path_tree_size(Path_tree *tree);
#endif
