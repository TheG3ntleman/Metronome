#ifndef AGENT_H
#define AGENT_H

#include <stdlib.h>
#include "state_space_tree.h"

typedef uint Solution;

typedef struct {

  uint timeslot;
  uint venue;

} TimeTableEntry;
typedef struct {

  Solution *solution;
  StateNode *current_node;
  uint depth;

  TimeTableEntry *timetable;

} Agent;

Agent *Agent_make(uint n_sessions);
void Agent_free(Agent *agent);

void Agent_move_to_child(Agent *agent, uint child_index);
  // Updates depth
  // Update branch vector
  // Updategs current node

#endif