#ifndef MONTE_CARLO_TREE_SEARCH_H
#define MONTE_CARLO_TREE_SEARCH_H

#include "../timetable/utils.h"
#include "../timetable/population.h"
#include "../timetable/specifications.h"

#include "state_space_tree.h"
#include "agent.h"
#include "feasibility.h"

typedef struct {

  uint n_sessions;
  uint n_solutions;
  TimeTableTuple **solution;

} MCTS_solution;

typedef struct {

  uint n_sessions;
  uint max_options;
  uint *n_options;

  TimeTableTuple **problem;

  TimeTableSpecifications *time_table_specifications;

} MCTS_problem;

typedef struct {

  uint n_branches_for_termination;
  snumeric exploration_factor;

} MCTS_solver_specifications;


MCTS_problem *MCTS_make_problem_from_population(
    Population *population, uint options_per_session,
    TimeTableSpecifications *time_table_specifications);
void MCTS_free_problem(MCTS_problem *problem);
void MCTS_print_problem(MCTS_problem *problem);

MCTS_solution *MCTS_solve(MCTS_problem *problem, MCTS_solver_specifications *specs);
void MCTS_print_solution(MCTS_solution *solution);
void MCTS_free_solution(MCTS_solution *solution);

#endif