#ifndef MONTE_CARLO_TREE_SEARCH_H
#define MONTE_CARLO_TREE_SEARCH_H

#include "../timetable/population.h"
#include "../timetable/specifications.h"
#include "agent.h"

#include <stdlib.h>

// typedef struct {

//   uint timeslot;
//   uint venue;

// } MCTS_option;
typedef struct {

  uint max_complete_branches;

  uint n_sessions;
  uint n_options;

  TimeTableEntry **problem;

  TimeTableSpecifications *time_table_specifications;

} MCTS_problem;

typedef struct {

  uint n_sessions;
  TimeTableEntry *solution;

} MCTS_solution;

MCTS_problem *MCTS_make_problem_from_population(
    Population *population, uint options_per_session,
    uint max_complete_branches,
    TimeTableSpecifications *time_table_specifications);
MCTS_solution *MCTS_execute(MCTS_problem *problem);
void MCTS_free_problem(MCTS_problem *problem);

//MCTS_solution *MCTS_make_solution(MCTS_problem *problem);
void MCTS_free_solution(MCTS_solution *solution);

#endif