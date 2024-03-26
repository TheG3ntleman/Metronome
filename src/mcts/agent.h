#ifndef AGENT_H
#define AGENT_H

#include "../timetable/utils.h"
#include "state_space_tree.h"

typedef struct  {

  // History of explored branch
  uint *branch_vector; // Option Indices (Essentially index of child node
                      // selected) for choices made at each level

  // Connection to tree
  StateSpaceTreeNode *current_node;

  // Internal Time Table State
  TimeTableTuple *timetable;

  // Control variables
  uint depth;
  uint max_depth;
} Agent;

Agent *Agent_make(uint max_depth);
Agent *Agent_clone(Agent *agent);
void Agent_free(Agent *agent);

// To move agent around/to a tree
void Agent_on_tree(Agent *agent, StateSpaceTree *tree);
void Agent_go_to(Agent *agent, uint option_index);
void Agent_go_back(Agent *agent);


#endif