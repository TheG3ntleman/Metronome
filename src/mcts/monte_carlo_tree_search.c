#include "monte_carlo_tree_search.h"
#include "src/mcts/agent.h"
#include "src/mcts/feasibility.h"
#include "src/mcts/state_space_tree.h"

#include <stdio.h>
#include <stdlib.h>

MCTS_problem *MCTS_make_problem_from_population(
    Population *population, uint options_per_session,
    TimeTableSpecifications *time_table_specifications) {
  MCTS_problem *problem = malloc(sizeof(MCTS_problem));

  problem->problem = malloc(population->n_sessions * sizeof(TimeTableTuple *));
  for (uint i = 0; i < population->n_sessions; i++) {
    problem->problem[i] = malloc(options_per_session * sizeof(TimeTableTuple));
  }

  problem->n_sessions = population->n_sessions;
  problem->n_options = (uint *)malloc(sizeof(uint) * population->n_sessions);
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

      // TODO: Add another if to break the loop that
      // implements the thresholding of the maximum frequency
      // For now using this, not sure if it is good
      if (max_frequency <= 5)
        break;

      if (max_frequency == 0) {
        break;
      }

      // Adding this to the problem
      problem->problem[i_session][problem->n_options[i_session]].timeslot =
          max_index_timeslot;
      problem->problem[i_session][problem->n_options[i_session]].venue =
          max_index_venue;
      problem->n_options[i_session]++;

      // Updating flag
      flag_table[max_index_timeslot][max_index_venue] = 1;
    }
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

void MCTS_free_problem(MCTS_problem *problem) {
  free(problem->n_options);
  for (uint i = 0; i < problem->n_sessions; i++) {
    free(problem->problem[i]);
  }
  free(problem->problem);
  free(problem);
}

void MCTS_print_problem(MCTS_problem *problem) {
  printf("MCTS Problem:\n");
  printf("  Number of Sessions: %d\n", problem->n_sessions);
  printf("  Maximum Number of Options: %d\n", problem->max_options);
  printf("  Session and corresponding options:\n\n");
  for (uint i = 0; i < problem->n_sessions; i++) {
    printf("session %d:\n", i);
    for (uint j = 0; j < problem->n_options[i]; j++) {
      printf("\toption %d: timeslot %d, venue %d\n", j,
             problem->problem[i][j].timeslot, problem->problem[i][j].venue);
    }
  }
}

MCTS_solution *MCTS_solve(MCTS_problem *problem, MCTS_solver_specifications *specs) {

  // Allocating memory for solution
  MCTS_solution *solution = malloc(sizeof(MCTS_solution));
  solution->n_sessions = problem->n_sessions;
  solution->n_solutions = specs->n_branches_for_termination;
  solution->solution =
      malloc(sizeof(TimeTableTuple *) * specs->n_branches_for_termination);
  for (uint i = 0; i < problem->n_sessions; i++) {
    solution->solution[i] =
        malloc(sizeof(TimeTableTuple) * problem->n_sessions);
  }

  // Making a state space tree
  StateSpaceTree *tree = StateSpaceTree_make();

  // Making an agent
  Agent *agent = Agent_make(problem->n_sessions);

  // Putting agent on tree
  Agent_on_tree(agent, tree);

  // Setting up main MCTS Loop

  for (uint n_found_solutions = 0;
       n_found_solutions < specs->n_branches_for_termination;) {

    while (agent->depth < problem->n_sessions) {

      if (!agent->current_node->children_generated) {
        // We check if the current node has any feasible children
        uint n_feasibile_actions;
        uint feasible_actions[problem->max_options];

        get_feasible_actions(agent, problem->time_table_specifications,
                            problem->problem[agent->depth],
                            problem->n_options[agent->depth],
                            feasible_actions, &n_feasibile_actions);

        printf("Number of feasible actions: %d\n", n_feasibile_actions);

        // Put this entire block into the StateSpaceTree_add_child_nodes function
        if (n_feasibile_actions == 0) {

          agent->current_node->feasible = FALSE;
          agent->current_node->children_generated = TRUE;
          agent->current_node->n_children = 0;

        } else {
          // Adding feasible children to node
          agent->current_node->feasible = TRUE;
          agent->current_node->children_generated = TRUE;
          agent->current_node->n_children = n_feasibile_actions;

          TimeTableTuple children_values[n_feasibile_actions];
          for (uint i = 0; i < n_feasibile_actions; i++) {
            children_values[i] = problem->problem[agent->depth][feasible_actions[i]];
          }
          StateSpaceTree_add_child_nodes(agent->current_node, n_feasibile_actions, children_values);
        }
      }

      if (!agent->current_node->feasible) {
        // Going to the last feasible point
        Agent_go_back(agent);

        // Checking if this node has any feasible children
        bool has_feasible_children = FALSE;
        for (uint i = 0; i < agent->current_node->n_children; i++) {
          if (agent->current_node->children[i]->feasible) {
            has_feasible_children = TRUE;
            break;
          }
        }

        if (!has_feasible_children) {
          // Going back to the last feasible point
          agent->current_node->feasible = FALSE;
        }
      }

      // Now we are sure that we are at a feasible node with expanded children.
      // We now ramdomly select a child node to go to

      uint selected_child_index = rand() % agent->current_node->n_children;
      Agent_go_to(agent, selected_child_index);

    }

    // Filling the solution
    for (uint i = 0; i < problem->n_sessions; i++) {
      solution->solution[n_found_solutions][i].timeslot =
          agent->timetable[i].timeslot;
      solution->solution[n_found_solutions][i].venue = agent->timetable[i].venue;
    }
  }

  // Freeing objects
  StateSpaceTree_free(tree);

  // Returning solution
  return solution;
}

void MCTS_print_solution(MCTS_solution *solution) {
  printf("MCTS Solution:\n");
  printf("  Number of Sessions: %d\n", solution->n_sessions);
  printf("  Number of Solutions: %d\n", solution->n_solutions);
  printf("  Solutions:\n\n");
  for (uint i = 0; i < solution->n_solutions; i++) {
    printf("solution %d:\n", i);
    for (uint j = 0; j < solution->n_sessions; j++) {
      printf("\tsession %d: timeslot %d, venue %d\n", j,
             solution->solution[i][j].timeslot, solution->solution[i][j].venue);
    }
  }
}

void MCTS_free_solution(MCTS_solution *solution) {
  for (uint i = 0; i < solution->n_solutions; i++)
    free(solution->solution[i]);
  free(solution->solution);
  free(solution);
}