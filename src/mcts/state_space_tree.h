#ifndef STATE_SPACE_TREE_H
#define STATE_SPACE_TREE_H

#include "../common/utils.h"

typedef struct StateSpaceTreeNode{

  // Option/Choice related variables
  uint option_index;
  TimeTableTuple option_value;

  // MCTS selection criteria
  uint n_visits;
  snumeric reward;

  // Related to Tree Structure (Parent)
  uint depth;
  struct StateSpaceTreeNode *parent;

  char children_generated, feasible; // Fertile can be removed for n_children == 0 but that would be less readable
  uint n_children;
  struct StateSpaceTreeNode **children;

} StateSpaceTreeNode;

typedef struct {

  // Main root node
  StateSpaceTreeNode *root;
  
  // Variable to track progress
  uint max_depth_attained;

} StateSpaceTree;

StateSpaceTree *StateSpaceTree_make();
void StateSpaceTree_free(StateSpaceTree *tree);

// Function to add a child nodes
void StateSpaceTree_add_child_nodes(StateSpaceTreeNode *parent, uint n_children, TimeTableTuple *children_values);

#endif