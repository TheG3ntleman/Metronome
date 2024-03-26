#ifndef BACKPROPAGATE_H
#define BACKPROPAGATE_H

#include "agent.h"
#include "state_space_tree.h"

void backpropagate(StateSpaceTree *state_space_tree, Solution *solution,
                   uint depth, snumeric reward);

#endif