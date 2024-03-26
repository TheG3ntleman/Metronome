#include "selection.h"
#include "agent.h"
#include "basic.h"

void select_parent_node(StateSpaceTree *state_space_tree, Agent *agent) {
  while(agent->current_node->children_expanded) {
    uint best_child_index = select_child_node(state_space_tree, agent);
    agent_move_to_child(agent, best_child_index);
  }
}

uint select_child_node(StateSpaceTree *state_space_tree, Agent *agent) {
  snumeric best_selection_factor = 0;
  uint best_child_index = 0;

  for (uint i = 0; i < agent->current_node->n_children; i++) {
    // Q/N + C * sqrt(ln(parent_visits) / N)
    StateNode *child = agent->current_node->children + i;
    snumeric selection_factor = child->reward / child->visits +
                                (1.0/EXPLORATION_FACTOR) *
                                sqrt(2.0 * log(agent->current_node->visits) / child->visits);
    if (selection_factor > best_selection_factor) {
      best_selection_factor = selection_factor;
      best_child_index = i;
    }
  }

  return best_child_index;
}