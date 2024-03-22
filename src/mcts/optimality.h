#ifndef OPTIMALITY_H
#define OPTIMALITY_H

#include "../timetable/specifications.h"
#include "agent.h"
#include "state_space_tree.h"

snumeric get_optimality(StateSpaceTree *state_space_tree, Agent *agent,
                              TimeTableSpecifications *specifications);

#endif