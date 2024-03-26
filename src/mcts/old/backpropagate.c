#include "backpropagate.h"

void backpropagate(StateSpaceTree *state_space_tree, Solution *solution,
                   uint depth, snumeric reward) {
  StateNode *node = state_space_tree->root;
  for (uint i = 0; node != NULL; i++) {
    node->reward += reward;
    node->visits++;

    node = node->children + solution[i];
  }
}