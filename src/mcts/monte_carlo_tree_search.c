#include "monte_carlo_tree_search.h"

/*
#include "agent.h"
#include "optimality.h"*/

#include <limits.h>
#include <stdlib.h>

#include "backpropagate.h"
#include "feasibility.h"
#include "selection.h"
#include "simulation.h"
#include "state_space_tree.h"

MCTS_problem *MCTS_make_problem_from_population(
    Population *population, uint options_per_session,
    uint max_complete_branches,
    TimeTableSpecifications *time_table_specifications) {
  MCTS_problem *problem = malloc(sizeof(MCTS_problem));

  problem->n_sessions = population->n_sessions;
  problem->n_options = options_per_session;
  problem->max_complete_branches = max_complete_branches;
  problem->time_table_specifications = time_table_specifications;

  // Sorting and ranking most frequent options
  // and then constructing the MCTS problem.

  return problem;
}

void MCTS_free_problem(MCTS_problem *problem) {}

MCTS_solution *MCTS_execute(MCTS_problem *problem) {

  // Making a state space tree
  StateSpaceTree *state_space_tree = StateSpaceTree_make();

  // Making an agent
  Agent *agent = Agent_make(problem->n_sessions);

  // Running the MCTS algorithm
  Solution solutions[problem->max_complete_branches][problem->n_sessions];

  for (uint n_solutions = 0; n_solutions < problem->max_complete_branches;) {

    // Selecting a parent node
    select_parent_node(state_space_tree, agent);

    if (agent->depth == problem->n_sessions) {
      // If the agent has reached the end of the tree
      // then we have a complete solution
      for (uint i = 0; i < problem->n_sessions; i++) {
        solutions[n_solutions][i] = agent->solution[i];
      }
      n_solutions++;
      agent->current_node->visits = UINT_MAX;
      continue;
    }

    // Check if this node has been expanded
    if (!agent->current_node->children_expanded) {
      uint n_feasible_children = 0;
      uint feasible_children[problem->n_options];

      get_feasible_actions(state_space_tree, agent,
                           problem->time_table_specifications,
                           feasible_children, &n_feasible_children);

      agent->current_node->n_children = n_feasible_children;
      agent->current_node->children =
          malloc(n_feasible_children * sizeof(StateNode));

      for (uint i = 0; i < n_feasible_children; i++) { 
        StateNode *child = agent->current_node->children + i;
        child->option = feasible_children[i];
        child->visits = 0;
        child->reward = 0;
        child->parent = agent->current_node;
        child->children_expanded = 0;
        child->n_children = 0;
        child->children = NULL;
      }
      
      agent->current_node->children_expanded = 1;
    }

    // Getting children for the parent node
    uint child_index = select_child_node(state_space_tree, agent);
    Agent_move_to_child(agent, child_index);

    // Simulating the child node
    snumeric reward = simulate(state_space_tree, agent, problem->time_table_specifications);

    // Backpropagating the reward
    backpropagate(state_space_tree, agent->solution, reward);
  }

  // Wrapping the solution in an MCTS Solution object
  MCTS_solution *mcts_solution = malloc(sizeof(MCTS_solution));
  mcts_solution->solution = (MCTS_option*)malloc(problem->n_sessions * sizeof(MCTS_option));

  mcts_solution->n_sessions = problem->n_sessions;
  StateNode *current_node = state_space_tree->root;
  for (uint i = 0; i < problem->n_sessions; i++) {
    current_node = current_node->children + solutions[0][i];
    mcts_solution->solution->timeslot = problem->problem[i][current_node->option].timeslot;
    mcts_solution->solution->venue = problem->problem[i][current_node->option].venue;
  }

  return mcts_solution;

}

void MCTS_free_solution(MCTS_solution *solution) {
  free(solution->solution);
  free(solution);
}