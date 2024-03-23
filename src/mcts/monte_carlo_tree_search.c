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


TimeTableEntry find_max(uint ***arr, uint n_timeslots, uint n_venues){
  uint mx = 0;
  uint n_timeslot_number = 0;
  uint n_venue_number = 0;
  for(uint i=0; i < n_timeslots; i++){
    for(uint j=0;j < n_venues;j++){
      if(arr[i][j][1] == 0 && arr[i][j][0] > mx){
        mx = arr[i][j][1];
        n_timeslot_number = i;
        n_venue_number = j;
      }
    }
  }

  arr[n_timeslot_number][n_venue_number][1] = 1;
  TimeTableEntry result = {n_timeslot_number, n_venue_number};

  return result;
}

MCTS_problem *MCTS_make_problem_from_population(
    Population *population, uint options_per_session,
    uint max_complete_branches,
    TimeTableSpecifications *time_table_specifications)
{
  MCTS_problem *problem = malloc(sizeof(MCTS_problem));

  problem->n_sessions = population->n_sessions;
  problem->n_options = options_per_session;
  problem->max_complete_branches = max_complete_branches;
  problem->time_table_specifications = time_table_specifications;


  uint frequency_table[population->n_timeslots][population->n_venues][2];

  for (uint i = 0; i < population->n_timeslots; i++) {
    for (uint j = 0; j < population->n_venues; j++) {
      frequency_table[i][j][0] = 0; // Corresponds to the counter
      frequency_table[i][j][1] = 0; // Corresponds to the Flag
    }
  }

  for(uint k = 0; k < population->n_timetables; k++){
    for(uint i = 0; i < population->n_sessions; i++){
      for(uint j = 0; j < options_per_session; j++){
        TimeTableEntry temp = {0,0};
        ttGetTuple(population, k, i, &temp.timeslot, &temp.venue);
        frequency_table[temp.timeslot][temp.venue][0]++;
      }
    }
  }
  uint max_no_of_sessions = 0;

  // n_options, n_sessions
  for(uint i = 0; i < population->n_sessions; i++){
    uint check = 0;
    for(uint j = 0; j < options_per_session; j++){
      TimeTableEntry temp = find_max((uint ***)frequency_table, population->n_timeslots, population->n_venues);
      problem->problem[i][j].timeslot = temp.timeslot;
      problem->problem[i][j].venue = temp.venue;
      max_no_of_sessions++;
      if(max_no_of_sessions == problem->n_sessions){
        check = 1;
        break;
      }
    }
    if(check == 1){
      break;
    }
  }

  return problem;

}

void MCTS_print_problem(MCTS_problem *problem) {
  printf("MCTS Problem:\n");
  printf("  n_sessions: %d\n", problem->n_sessions);
  printf("  n_options: %d\n", problem->n_options);
  printf("  max_complete_branches: %d\n", problem->max_complete_branches);
  printf("  problem:\n");
  for (uint i = 0; i < problem->n_sessions; i++) {
    printf("    session %d:\n", i);
    for (uint j = 0; j < problem->n_options; j++) {
      printf("      option %d: timeslot %d, venue %d\n", j, problem->problem[i][j].timeslot, problem->problem[i][j].venue);
    }
  }

}


void MCTS_free_problem(MCTS_problem *problem) {}

MCTS_solution *MCTS_execute(MCTS_problem *problem)
{

  // Making a state space tree
  StateSpaceTree *state_space_tree = StateSpaceTree_make();

  // Making an agent
  Agent *agent = agent_make(problem->n_sessions);

  // Running the MCTS algorithm
  Solution solutions[problem->max_complete_branches][problem->n_sessions];

  for (uint n_solutions = 0; n_solutions < problem->max_complete_branches;)
  {

    // Selecting a parent node
    select_parent_node(state_space_tree, agent);

    if (agent->depth == problem->n_sessions)
    {
      // If the agent has reached the end of the tree
      // then we have a complete solution
      for (uint i = 0; i < problem->n_sessions; i++)
      {
        solutions[n_solutions][i] = agent->solution[i];
      }
      n_solutions++;
      agent->current_node->visits = UINT_MAX;
      continue;
    }

    // Check if this node has been expanded
    if (!agent->current_node->children_expanded)
    {
      uint n_feasible_children = 0;
      uint feasible_children[problem->n_options];

      /*StateSpaceTree *state_space_tree, Agent *agent,
                            TimeTableSpecifications *specifications,
                            TimeTableEntry *options, uint n_options, uint
         *actions, uint *n_actions*/

      // Extracting all possible options from the MCTS Problem
      TimeTableEntry options[problem->n_options];
      for (uint i = 0; i < problem->n_options; i++)
      {
        options[i].timeslot = problem->problem[i]->timeslot;
        options[i].venue = problem->problem[i]->venue;
      }

      get_feasible_actions(
          agent, problem->time_table_specifications, options,
          problem->n_options, feasible_children, &n_feasible_children);

      agent->current_node->n_children = n_feasible_children;
      agent->current_node->children =
          malloc(n_feasible_children * sizeof(StateNode));

      for (uint i = 0; i < n_feasible_children; i++)
      {
        StateNode *child = agent->current_node->children + i;
        child->option = feasible_children[i];
        child->visits = 0;
        child->reward = 0;
        child->parent = agent->current_node;
        child->children_expanded = 0;
        child->n_children = 0;
        child->children = NULL;
        
        // There is a chance of an indexing problem here, 
        // because of agent->depth + 1 may be incorrect
        // due to 0 indexing.
        child->choice.timeslot = problem->problem[agent->depth + 1][feasible_children[i]].timeslot;
        child->choice.venue = problem->problem[agent->depth + 1][feasible_children[i]].venue;
      }

      agent->current_node->children_expanded = 1;
    }

    // Getting children for the parent node
    uint child_index = select_child_node(state_space_tree, agent);
    agent_move_to_child(agent, child_index);

    // Simulating the child node
    snumeric reward =
        simulate(state_space_tree, agent, problem->time_table_specifications, problem);

    // Backpropagating the reward
    backpropagate(state_space_tree, agent->solution, agent->depth, reward);
  }

  // Wrapping the solution in an MCTS Solution object
  MCTS_solution *mcts_solution = malloc(sizeof(MCTS_solution));
  mcts_solution->solution =
      (TimeTableEntry *)malloc(problem->n_sessions * sizeof(TimeTableEntry));

  mcts_solution->n_sessions = problem->n_sessions;
  StateNode *current_node = state_space_tree->root;
  for (uint i = 0; i < problem->n_sessions; i++)
  {
    current_node = current_node->children + solutions[0][i];
    mcts_solution->solution->timeslot =
        problem->problem[i][current_node->option].timeslot;
    mcts_solution->solution->venue =
        problem->problem[i][current_node->option].venue;
  }

  return mcts_solution;
}

void MCTS_free_solution(MCTS_solution *solution)
{
  free(solution->solution);
  free(solution);
}