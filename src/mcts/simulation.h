#ifndef SIMULATION_H
#define SIMULATION_H

#include "../timetable/specifications.h"
#include "agent.h"
#include "state_space_tree.h"

snumeric simulate(StateSpaceTree *state_space_tree, Agent *agent, TimeTableSpecifications *time_table_specifications);

#endif