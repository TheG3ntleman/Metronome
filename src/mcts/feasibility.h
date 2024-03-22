#ifndef FEASIBILITY_H
#define FEASIBILITY_H

#include "../timetable/specifications.h"
#include "agent.h"
#include "state_space_tree.h"

void get_feasible_actions(StateSpaceTree *state_space_tree, Agent *agent,
                          TimeTableSpecifications *specifications,
                          TimeTableEntry *options, uint n_options, uint *actions,
                          uint *n_actions);

#endif