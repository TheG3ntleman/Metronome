#ifndef AGENT_H
#define AGENT_H

#include "../timetable/config.h"
#include "../timetable/utils.h"
#include "state_space_tree.h"

typedef struct {
  // Populate this with the necessary information
} PartialTimeTable;

typedef struct{

  uint n_complete_vectors;
  uint max_complete_vectors;

  PartialTimeTable *solutions; // An array of "max_complete_vectors" PartialTimeTables.
  uint *solution_vector;

  StateSpaceTree *tree;
} Agent;

uint select_action(Agent *agent);
numeric reward(Agent *agent, uint action);
void simulate(Agent *agent, uint action);
uint heuristic(Agent *agent, uint *feasible_actions, uint n_feasible_actions);
void backpropagate(Agent *agent, uint action, numeric reward);

#endif