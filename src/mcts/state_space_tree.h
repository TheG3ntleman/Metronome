#ifndef STATE_SPACE_TREE_H
#define STATE_SPACE_TREE_H

#include "../timetable/utils.h"

typedef struct StateNode {

  snumeric reward;
  uint counter;

  struct StateNode *children; // The number of tuples per session is also the number of children per node.


} StateNode;

typedef struct {

  StateNode *root;

} StateSpaceTree;

#endif