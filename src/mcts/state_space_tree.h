#ifndef STATE_SPACE_TREE_H
#define STATE_SPACE_TREE_H

#include "../timetable/utils.h"
#include <stdlib.h>

typedef uint Counter;

typedef struct StateNode{

  uint option;

  Counter visits;
  snumeric reward;

  struct StateNode *parent;

  char children_expanded;

  uint n_children;
  struct StateNode *children;

} StateNode;

typedef struct {

  StateNode *root;  

} StateSpaceTree;

StateSpaceTree *StateSpaceTree_make();
void StateSpaceTree_free(StateSpaceTree *tree);

#endif