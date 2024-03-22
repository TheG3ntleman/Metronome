#include "agent.h"

Agent *Agent_make(uint n_sessions) {
    Agent *agent = malloc(sizeof(Agent));

    agent->solution = (uint*) malloc(sizeof(uint) * n_sessions);

    return agent;
}

void Agent_free(Agent *agent) {
    free(agent);
}