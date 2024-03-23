#include "agent.h"
#include "feasibility.h"
#include "monte_carlo_tree_search.h"

typedef char bool;

bool hard_constraint_timeslot_venue(TimeTableEntry *time_table,
                                    TimeTableEntry *option, uint n_sessions,
                                    TimeTableSpecifications *specs) {
  // If this option leads to violation then return 0 otherwise return 1
}

bool is_feasible(TimeTableEntry *time_table, TimeTableEntry *option,
                 uint n_sessions, TimeTableSpecifications *specs) {
  if (!hard_constraint_timeslot_venue(time_table, option, n_sessions, specs))
    return 0;

  // Add other hard constraint ifs

  return 1;
}

void get_feasible_actions(Agent *agent, TimeTableSpecifications *specifications,
                          TimeTableEntry *options, uint n_options,
                          uint *actions, uint *n_actions) {

  // We expect options to be an array of size n_options

  // Getting partial time table for this agent state
  // agent->timetable[i].timeslot

  // Iterating through all possible options here.
  for (uint i = 0; i < n_options; i++) {
    if (is_feasible(agent->timetable, options + i, agent->depth,
                    specifications)) {
      actions[*n_actions] = i;
      (*n_actions)++;
    }
  }
}