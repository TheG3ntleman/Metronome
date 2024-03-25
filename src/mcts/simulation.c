#include "simulation.h"
#include "basic.h"
#include "monte_carlo_tree_search.h"
#include "optimality.h"
#include "state_space_tree.h"

snumeric simulate(StateSpaceTree *state_space_tree, Agent *agent,
                  TimeTableSpecifications *time_table_specifications,
                  MCTS_problem *problem) {

START_SIMULATION_AGAIN:

  // Making a temporary copy of the agent.
  Agent *t_agent = agent_clone(agent);
  t_agent->current_node = NULL; // this is not used

  for (; t_agent->depth < time_table_specifications->session_table->size;) {

    // Getting feasible actions for the current node
    uint n_feasible_children = 0;
    uint feasible_children[problem->n_options];

    /*StateSpaceTree *state_space_tree, Agent *agent,
                          TimeTableSpecifications *specifications,
                          TimeTableEntry *options, uint n_options, uint
       *actions, uint *n_actions*/

    // Extracting all possible options from the MCTS Problem
    TimeTableEntry options[problem->n_options];
    for (uint i = 0; i < problem->n_options; i++) {
      options[i].timeslot = problem->problem[i]->timeslot;
      options[i].venue = problem->problem[i]->venue;
    }

    get_feasible_actions(t_agent, problem->time_table_specifications, options,
                         problem->n_options, feasible_children,
                         &n_feasible_children);

    if (n_feasible_children == 0) {

      // Calling soft contraint optimality functions
      // to check if the current solution is optimal
      // or not.

      snumeric reward =
          get_optimality(agent->timetable, agent->depth, problem->time_table_specifications);
      snumeric k = (problem->n_sessions - agent->depth) / (t_agent->depth - agent->depth);
      backpropagate(state_space_tree, agent->solution, agent->depth,
                    reward / k);

      goto START_SIMULATION_AGAIN;
    }

    // using a random selection heuristic for now
    uint random_index = rand() % n_feasible_children;

    // Updating agent state.
    t_agent->solution[t_agent->depth] = random_index;
    t_agent->timetable[t_agent->depth].timeslot =
        problem->problem[t_agent->depth][feasible_children[random_index]]
            .timeslot;
    t_agent->timetable[t_agent->depth].venue =
        problem->problem[t_agent->depth][feasible_children[random_index]].venue;

    t_agent->depth++;
  }

  // Getting optimality of the solution
  snumeric reward = get_optimality(agent->timetable, agent->depth, time_table_specifications);

  agent_free(t_agent);

  return reward;
}
