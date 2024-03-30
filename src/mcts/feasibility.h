#ifndef FEASIBILITY_H
#define FEASIBILITY_H

#include "../common/specifications.h"
#include "agent.h"
#include "state_space_tree.h"

// DONT PASS AGENT.
void get_feasible_actions(Agent *agent, TimeTableSpecifications *specifications,
                          TimeTableTuple *options, uint n_options,
                          uint *actions, uint *n_actions);

#endif