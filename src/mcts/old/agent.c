#include "agent.h"
#include "state_space_tree.h"

Agent *agent_make(uint n_sessions) {

  Agent *agent = (Agent *)malloc(sizeof(Agent));

  agent->depth = 0;
  agent->current_node = NULL;
  agent->solution = (Solution *)malloc(n_sessions * sizeof(Solution));
  agent->timetable =
      (TimeTableEntry *)malloc(n_sessions * sizeof(TimeTableEntry));

  return agent;
}

void agent_free(Agent *agent) {
  free(agent->solution);
  free(agent->timetable);
  free(agent);
}

void agent_move_to_child(Agent *agent, uint child_index) {
  agent->current_node = agent->current_node->children + child_index;

  agent->timetable[agent->depth].timeslot = agent->current_node->choice.timeslot;
  agent->timetable[agent->depth].venue = agent->current_node->choice.venue;
  agent->solution[agent->depth] = child_index;

  agent->depth++;
}

Agent *agent_clone(Agent *agent) {

  Agent *clone = (Agent *)malloc(sizeof(Agent));

  clone->depth = agent->depth;
  clone->current_node = agent->current_node;
  clone->solution = (Solution *)malloc(agent->depth * sizeof(Solution));
  clone->timetable = (TimeTableEntry *)malloc(agent->depth * sizeof(TimeTableEntry));

  for (uint i = 0; i < agent->depth; i++) {
    clone->solution[i] = agent->solution[i];
    clone->timetable[i].timeslot = agent->timetable[i].timeslot;
    clone->timetable[i].venue = agent->timetable[i].venue;
  }

  return clone;

}