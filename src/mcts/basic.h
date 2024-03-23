#ifndef BASIC_H
#define BASIC_H

#include <stdlib.h>
#include "../timetable/specifications.h"
#include "agent.h"
#include "optimality.h"

#define EXPLORATION_FACTOR 1.41421356237

typedef struct {

  uint max_complete_branches;

  uint n_sessions;
  uint n_options;

  TimeTableEntry **problem;

  TimeTableSpecifications *time_table_specifications;

} MCTS_problem;

#endif