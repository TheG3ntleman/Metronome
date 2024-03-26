#include "state_space_tree.h"

StateSpaceTree *StateSpaceTree_make() {

  StateSpaceTree *tree = malloc(sizeof(StateSpaceTree));

  // Filling the fields of the tree struct
  tree->max_depth_attained = 0;
  tree->root = malloc(sizeof(StateSpaceTreeNode));

  // Filling the fields of the state space tree node struct (representative of
  // the root)
  tree->root->option_index = 0;          // The root node has no option index
  tree->root->option_value.timeslot = 0; // The root node has no option value
  tree->root->option_value.venue = 0;    // The root node has no option value

  tree->root->n_visits = 0;
  tree->root->reward = 0;

  tree->root->depth = 0;
  tree->root->parent = NULL;

  tree->root->children_generated = FALSE;
  tree->root->feasible = TRUE; // We assume that every node is feasible until it is not.
  tree->root->n_children = 0;
  tree->root->children = NULL;

  return tree;
}

void StateSpaceTree_free(StateSpaceTree *tree) {

  // Making a stack to store the nodes
}

// Function to add a child nodes
void StateSpaceTree_add_child_nodes(StateSpaceTreeNode *parent, uint n_children,
                                    TimeTableTuple *children_values) {

  // Allocating memory for the children
  parent->children_generated = TRUE;
  parent->n_children = n_children;


}