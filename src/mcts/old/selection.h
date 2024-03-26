#ifndef SELECTION_H
#define SELECTION_H

#include "../timetable/specifications.h"
#include "agent.h"
#include "state_space_tree.h"
#include <math.h>

void select_parent_node(StateSpaceTree *state_space_tree, Agent *agent);
uint select_child_node(StateSpaceTree *state_space_tree, Agent *agent);

#endif