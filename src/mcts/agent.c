#include "agent.h"
#include "src/mcts/monte_carlo_tree_search.h"
#include "src/mcts/state_space_tree.h"
#include <stdio.h>

Agent *Agent_make(uint max_depth) {
  Agent *agent = (Agent *)malloc(sizeof(Agent));

  agent->branch_vector = (uint *)malloc(sizeof(uint) * max_depth);
  agent->timetable =
      (TimeTableTuple *)malloc(sizeof(TimeTableTuple) * max_depth);
  agent->current_node = NULL;
  agent->max_depth = max_depth;
  agent->depth = 0;

  return agent;
}

Agent *Agent_clone(Agent *agent) {

  Agent *clone = (Agent *)malloc(sizeof(Agent));

  clone->branch_vector = (uint *)malloc(sizeof(uint) * agent->max_depth);
  clone->timetable =
      (TimeTableTuple *)malloc(sizeof(TimeTableTuple) * agent->max_depth);
  clone->current_node = agent->current_node;
  clone->max_depth = agent->max_depth;
  clone->depth = agent->depth;

  return clone;
}

void Agent_free(Agent *agent) {

  free(agent->branch_vector);
  free(agent->timetable);
  free(agent);
}

void Agent_on_tree(Agent *agent, StateSpaceTree *tree) {

  agent->current_node = tree->root;
  agent->depth = 0;
}

void Agent_go_to(Agent *agent, uint option_index) {

  printf("Agent at depth: %d\n", agent->depth);
  printf("Most recent choice (timeslot, venue): (%d, %d)\n",
         agent->timetable[agent->depth - 1].timeslot,
         agent->timetable[agent->depth - 1].venue);
  printf("Option chosen: %d\n", option_index);
  printf("\n\n");

  if (agent->depth == agent->max_depth) {
    fprintf(stderr, "Cannot advance agent after already at maximum depth.\n");
    exit(-1);
  }

  agent->branch_vector[agent->depth] = option_index;
  agent->current_node = agent->current_node->children[option_index];

  agent->timetable[agent->depth].timeslot =
      agent->current_node->option_value.timeslot;
  agent->timetable[agent->depth].venue =
      agent->current_node->option_value.venue;

  agent->depth++;
}

void Agent_go_back(Agent *agent) {

  if (agent->depth == 0) {
    fprintf(stderr, "Cannot go back from root node.\n");
    exit(-1);
  }

  agent->depth--;
  agent->current_node = agent->current_node->parent;
}