#ifndef BACKPROPAGATE_H
#define BACKPROPAGATE_H

#include "state_space_tree.h"
#include "agent.h"

void backpropagate(StateSpaceTree *state_space_tree, Solution *solution, snumeric reward);

#endif