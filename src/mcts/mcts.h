#ifndef MCTS_H
#define MCTS_H

#include "../timetable/utils.h"
#include "agent.h"

typedef struct {
    uint n_time_tables;
    uint n_sessions;
    uint n_selections;

    // A 2 dimensional array of (timeslot, venue) tuples/pairs
    // First dimension - Iterates through sessions
    // Second Dimension Iterates through possible (ranked asc.) pair options
    uint *problem;
} MCTSProblem;


PartialTimeTable solve_mcts_problem(MCTSProblem *problem, uint n_complete_vectors /*how many vectors need to reach the end to terminate*/);

#endif