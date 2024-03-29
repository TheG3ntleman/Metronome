#ifndef NORMALIZATION_H
#define NORMALIZATION_H

#include "basic.h"
#include "monte_carlo_tree_search.h"
#include "optimality.h"
#include "state_space_tree.h"

snumeric normalize(StateSpaceTree *state_space_tree, Agent *agent,
                   TimeTableSpecifications *time_table_specifications,
                   MCTS_problem *problem, snumeric *maximum, snumeric *minimum);

#endif
