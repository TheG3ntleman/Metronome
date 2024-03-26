#include "monte_carlo_tree_search.h"

/*
#include "agent.h"
#include "optimality.h"*/

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "backpropagate.h"
#include "feasibility.h"
#include "selection.h"
#include "simulation.h"
#include "src/mcts/optimality.h"
#include "src/timetable/population.h"
#include "state_space_tree.h"

TimeTableEntry find_max(uint ***arr, uint n_timeslots, uint n_venues) {
  uint mx = 0;
  uint n_timeslot_number = 0;
  uint n_venue_number = 0;
  for (uint i = 0; i < n_timeslots; i++) {
    for (uint j = 0; j < n_venues; j++) {
      if (arr[i][j][1] == 0 && arr[i][j][0] > mx) {
        mx = arr[i][j][0];
        n_timeslot_number = i;
        n_venue_number = j;
      }
    }
  }

  arr[n_timeslot_number][n_venue_number][1] = 1; // already used
  TimeTableEntry result = {n_timeslot_number, n_venue_number};

  return result;
}

MCTS_problem *MCTS_make_problem_from_population(
    Population *population, uint options_per_session,
    uint max_complete_branches,
    TimeTableSpecifications *time_table_specifications) {
  MCTS_problem *problem = malloc(sizeof(MCTS_problem));

  problem->problem = malloc(population->n_sessions * sizeof(TimeTableEntry *));
  for (uint i = 0; i < population->n_sessions; i++) {
    problem->problem[i] = malloc(options_per_session * sizeof(TimeTableEntry));
  }

  problem->n_sessions = population->n_sessions;
  problem->n_options = (uint*)malloc(sizeof(uint) * population->n_sessions);
  problem->max_complete_branches = max_complete_branches;
  problem->time_table_specifications = time_table_specifications;

  // Maintaining a frequency table
  uint **frequency_table = malloc(sizeof(uint *) * population->n_timeslots);
  for (uint i = 0; i < population->n_timeslots; i++) {
    frequency_table[i] = malloc(sizeof(uint) * population->n_venues);
  }

  // Making a corresponding flag table
  uint **flag_table = malloc(sizeof(uint *) * population->n_timeslots);
  for (uint i = 0; i < population->n_timeslots; i++) {
    flag_table[i] = malloc(sizeof(uint) * population->n_venues);
  }

  for (uint i_session = 0; i_session < population->n_sessions; i_session++) {

    printf("For Session %u\n", i_session);

    // Initializing the frequency table and flag table
    for (uint i = 0; i < population->n_timeslots; i++) {
      for (uint j = 0; j < population->n_venues; j++) {
        frequency_table[i][j] = 0;
        flag_table[i][j] = 0;
      }
    }

    // Iterating through the timetables in the population
    for (uint j = 0; j < population->n_timetables; j++) {
      uint timeslot, venue;
      ttGetTuple(population, j, i_session, &timeslot, &venue);
      //printf("Time Table (for_session %u): (%u, %u)\n", i_session, timeslot, venue);
      frequency_table[timeslot][venue]++;
    }

    problem->n_options[i_session] = 0;

    // Finding the maximum frequency
    for (uint i = 0; i < options_per_session; i++) {
      // Finding maximum in frequency table
      uint max_frequency = 0;
      uint max_index_timeslot = 0;
      uint max_index_venue = 0;
      for (uint j = 0; j < population->n_timeslots; j++) {
        for (uint k = 0; k < population->n_venues; k++) {
          if (flag_table[j][k] == 0) {
            if (frequency_table[j][k] > max_frequency) {
              max_frequency = frequency_table[j][k];
              max_index_timeslot = j;
              max_index_venue = k;
            }
          }
        }
      }

      if (max_frequency == 0) {
        break;
      }

      // Adding this to the problem
      printf("\tTimeslot: %u, Venue: %u, Frequency: %u\n", max_index_timeslot, max_index_venue, max_frequency);
      problem->problem[i_session][problem->n_options[i_session]].timeslot = max_index_timeslot;
      problem->problem[i_session][problem->n_options[i_session]].venue = max_index_venue;
      problem->n_options[i_session]++;

      // Updating flag
      flag_table[max_index_timeslot][max_index_venue] = 1;
    }

    // Printing the frequency table
    // printf("For Session: %u\n", i_session);
    // for (uint i = 0; i < population->n_timeslots; i++) {
    //   for (uint j = 0; j < population->n_venues; j++) {
    //     printf("\t (%u, %u): %u\n", i, j, frequency_table[i][j]);
    //   }
    //   printf("\n");
    // }
    printf("\n\n");
  }

  // Freeing the frequency and flag tables
  for (uint i = 0; i < population->n_timeslots; i++) {
    free(frequency_table[i]);
    free(flag_table[i]);
  }
  free(frequency_table);
  free(flag_table);

  return problem;
}

void MCTS_print_problem(MCTS_problem *problem) {
  printf("MCTS Problem:\n");
  printf("  n_sessions: %d\n", problem->n_sessions);
  printf("  max_options: %d\n", problem->max_options);
  printf("  max_complete_branches: %d\n", problem->max_complete_branches);
  printf("  problem:\n");
  for (uint i = 0; i < problem->n_sessions; i++) {
    printf("    session %d:\n", i);
    for (uint j = 0; j < problem->n_options[i]; j++) {
      printf("      option %d: timeslot %d, venue %d\n", j,
             problem->problem[i][j].timeslot, problem->problem[i][j].venue);
    }
  }
}

void MCTS_free_problem(MCTS_problem *problem) {}

MCTS_solution *MCTS_execute(MCTS_problem *problem) {

  // Making a state space tree
  StateSpaceTree *state_space_tree = StateSpaceTree_make();

  // Making an agent
  Agent *agent = agent_make(problem->n_sessions);

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
      uint feasible_children[problem->n_options[agent->depth - 1]];

      /*StateSpaceTree *state_space_tree, Agent *agent,
                            TimeTableSpecifications *specifications,
                            TimeTableEntry *options, uint n_options, uint
         *actions, uint *n_actions*/

      // Extracting all possible options from the MCTS Problem
      TimeTableEntry options[problem->n_options[agent->depth - 1]];
      for (uint i = 0; i < problem->n_options; i++) {
        options[i].timeslot = problem->problem[i]->timeslot;
        options[i].venue = problem->problem[i]->venue;
      }

      get_feasible_actions(agent, problem->time_table_specifications, options,
                           problem->n_options[agent->depth - 1], feasible_children,
                           &n_feasible_children);

      agent->current_node->n_children = n_feasible_children;
      agent->current_node->children =
          malloc(n_feasible_children * sizeof(StateNode));

      if (n_feasible_children == 0) {
        agent->current_node->children_expanded = 1;

        // Calling soft contraint optimality functions
        // to check if the current solution is optimal
        // or not.

        snumeric reward = get_optimality(agent->timetable, agent->depth,
                                         problem->time_table_specifications);
        snumeric k = ((snumeric)agent->depth / problem->n_sessions) - 1;
        backpropagate(state_space_tree, agent->solution, agent->depth,
                      reward * k);

        continue;
      }

      for (uint i = 0; i < n_feasible_children; i++) {
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
        child->choice.timeslot =
            problem->problem[agent->depth + 1][feasible_children[i]].timeslot;
        child->choice.venue =
            problem->problem[agent->depth + 1][feasible_children[i]].venue;
      }

      agent->current_node->children_expanded = 1;
    }

    // Getting children for the parent node
    uint child_index = select_child_node(state_space_tree, agent);
    agent_move_to_child(agent, child_index);

    // Simulating the child node
    snumeric reward = simulate(state_space_tree, agent,
                               problem->time_table_specifications, problem);

    // Backpropagating the reward
    backpropagate(state_space_tree, agent->solution, agent->depth, reward);
  }

  // Wrapping the solution in an MCTS Solution object
  MCTS_solution *mcts_solution = malloc(sizeof(MCTS_solution));
  mcts_solution->solution =
      (TimeTableEntry *)malloc(problem->n_sessions * sizeof(TimeTableEntry));

  mcts_solution->n_sessions = problem->n_sessions;
  StateNode *current_node = state_space_tree->root;
  for (uint i = 0; i < problem->n_sessions; i++) {
    current_node = current_node->children + solutions[0][i];
    mcts_solution->solution->timeslot =
        problem->problem[i][current_node->option].timeslot;
    mcts_solution->solution->venue =
        problem->problem[i][current_node->option].venue;
  }

  return mcts_solution;
}

void MCTS_free_solution(MCTS_solution *solution) {
  free(solution->solution);
  free(solution);
}