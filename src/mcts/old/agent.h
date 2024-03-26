#ifndef AGENT_H
#define AGENT_H

#include <stdlib.h>
#include "state_space_tree.h"

typedef uint Solution;
typedef struct {
  uint depth;
  StateNode *current_node;

  Solution *solution;

  TimeTableEntry *timetable;
} Agent;

Agent *agent_make(uint n_sessions);
void agent_free(Agent *agent);
Agent *agent_clone(Agent *agent);

// This function could take something analagous to the problem or something
// Remember the first shitty option, it turns out that that was better.
void agent_move_to_child(Agent *agent, uint child_index);

  // Updates depth
  // Update branch vector
  // Updategs current node

#endif