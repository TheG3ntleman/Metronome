#include "feasibility.h"

#include "agent.h"
#include "feasibility.h"
#include <stdio.h>
#include <stdlib.h>

/* Two function need to be implemented:
  1) find_parties_from_session: given a session this function given all the
  parties participating in that session in the partially completed time table.
  2) find_sessions_from_parties: given a party this function given all the
  sessions that this party is participating in the partially completed time
  table. 3) Get_tuple: implemntint a function that takes the agent and option
  and gives timeslot, venue */

void Get_tuple(Agent *agent, TimeTableTuple *option, uint i, uint *venue_id,
               uint *timeslot_id) {
  // implement
  if (i < agent->depth) {
    *timeslot_id = agent->timetable[i].timeslot;
    *venue_id = agent->timetable[i].venue;
    return;
  } else if (i == agent->depth + 1) {
    *timeslot_id = option->timeslot;
    *venue_id = option->venue;
    return;
  } else {
    printf("Invalid get tuple call.\n");
    exit(-1);
  }
}

bool hard_constraint_timeslot_venue(TimeTableTuple *timetable, Agent *agent,
                                    TimeTableTuple *option, uint n_sessions,
                                    TimeTableSpecifications *specs) {

  // getting tiemslot and venue for option
  uint timeslot_id = option->timeslot;
  uint venue_id = option->venue;

  // If this option leads to violation then return 0 otherwise return 1
  for (uint i = 0; i < agent->depth; i++) {
    // getting timeslots and venues for agent
    uint timeslot_id1 = agent->timetable[i].timeslot;
    uint venue_id1 = agent->timetable[i].venue;

    if (timeslot_id1 == timeslot_id && venue_id1 == venue_id) {
      return FALSE;
    }
  }
  return TRUE;
}

bool hard_constraint_max_capacity(TimeTableTuple *timetable, Agent *agent,
                                  TimeTableTuple *option, uint n_sessions,
                                  TimeTableSpecifications *specs) {
  uint venue_id = option->venue;
  uint capacity = specs->venue_table->capacity[venue_id];

  uint party_id_array[specs->party_table->size];
  uint number_of_parties;
  uint strength = 0;
  findAssociatedParties(agent->depth + 1, &number_of_parties, party_id_array,
                        agent->depth + 1, specs);

  for (uint j = 0; j < number_of_parties; j++) {
    strength += specs->party_table->strength[party_id_array[j]];
  }

  printf("Strewngth: %d, Capacity: %d\n", strength, capacity);

  if (strength > capacity) {
    return FALSE;
  }

  return TRUE;
}

bool hard_constraint_venue_type(TimeTableTuple *timetable, Agent *agent,
                                TimeTableTuple *option, uint n_sessions,
                                TimeTableSpecifications *specs) {

  uint timeslot_id = option->timeslot;
  uint venue_id = option->venue;

  // Retrieve required and assigned venue type
  uint venue_type_assigned = specs->venue_table->type[venue_id];
  uint venue_type_required = specs->session_table->type[agent->depth + 1];

  // Check if they are matching
  if (venue_type_assigned != venue_type_required) {
    return FALSE;
  }

  return TRUE;
}

bool hard_constraint_max_sessions(TimeTableTuple *timetable, Agent *agent,
                                  TimeTableTuple *option, uint n_sessions,
                                  TimeTableSpecifications *specs) {

  uint number_of_days =
      specs->timeslot_table->day[specs->timeslot_table->size - 1];

  uint party_id_array[specs->party_table->size];
  uint number_of_parties;
  findAssociatedParties(agent->depth + 1, &number_of_parties, party_id_array,
                        agent->depth + 1, specs);

  uint day_array[number_of_days];
  for (uint j = 0; j < number_of_days; j++) {
    day_array[j] = 0;
  }

  for (uint i = 0; i < number_of_parties; i++) {

    uint session_id_array[specs->session_table->size];
    uint number_of_sessions;
    findAssociatedSessions(party_id_array[i], &number_of_sessions,
                           session_id_array, agent->depth + 1, specs);

    for (uint j = 0; j < number_of_sessions; j++) {
      uint timeslot_id, venue_id;
      Get_tuple(agent, option, session_id_array[j], &timeslot_id, &venue_id);
      uint day = specs->timeslot_table->day[timeslot_id];
      day_array[day]++;
    }

    for (uint j = 0; j < number_of_days; j++) {
      if (day_array[j] > specs->party_table->max_hours[i]) {
        return FALSE;
      }
      day_array[j] = 0;
    }
  }

  return TRUE;
}

bool hard_constraint_party_duplicate(TimeTableTuple *timetable, Agent *agent,
                                     TimeTableTuple *option, uint n_sessions,
                                     TimeTableSpecifications *specs) {

  uint party_id_array[specs->party_table->size];
  uint number_of_parties;
  findAssociatedParties(agent->depth + 1, &number_of_parties, party_id_array,
                        agent->depth + 1, specs);

  uint timeslot_array[specs->timeslot_table->size];
  for (uint i = 0; i < specs->timeslot_table->size; i++) {
    timeslot_array[i] = 0;
  }

  for (uint i = 0; i < number_of_parties; i++) {

    uint session_id_array[specs->session_table->size];
    uint number_of_sessions;
    findAssociatedSessions(party_id_array[i], &number_of_sessions,
                           session_id_array, agent->depth + 1, specs);

    for (uint j = 0; j < number_of_sessions; j++) {
      uint timeslot_id, venue_id;
      Get_tuple(agent, option, session_id_array[j], &timeslot_id, &venue_id);
      timeslot_array[j] = timeslot_id;
    }

    for (uint j = 0; j < number_of_sessions; j++) {
      for (uint k = j + 1; k < number_of_sessions; k++) {
        if (timeslot_array[j] == timeslot_array[k]) {
          return FALSE;
        }
      }
    }
  }

  return TRUE;
}

// make sure two sessions which have the same duration dont come sameltaneously
// in our timetable
bool hard_constraint_sufficient_timeslot(TimeTableTuple *timetable,
                                         Agent *agent, TimeTableTuple *option,
                                         uint n_sessions,
                                         TimeTableSpecifications *specs) {

  uint duration_option = specs->session_table->duration[agent->depth + 1];
  uint venue_id_option = option->venue;

  for (uint i = 0; i < duration_option; i++) {
    uint duration = specs->session_table->duration[agent->depth + 1 - 1 - i];

    if (duration_option == duration) {
      uint venue_id = agent->timetable[agent->depth + 1 - 1 - i].venue;
      if (venue_id != venue_id_option) {
        return FALSE;
      }
    } else {
      break;
    }
  }

  return TRUE;
}

bool is_feasible(TimeTableTuple *time_table, Agent *agent,
                 TimeTableTuple *option, uint n_sessions,
                 TimeTableSpecifications *specs) {

  if (!hard_constraint_timeslot_venue(time_table, agent, option, n_sessions,
                                      specs)) {
    return 0;
  }

  if (!hard_constraint_max_capacity(time_table, agent, option, n_sessions,
                                    specs)) {
    return 0;
  }
    printf("1234.\n");

  if (!hard_constraint_party_duplicate(time_table, agent, option, n_sessions,
                                       specs)) {
    return 0;
  }

  if (!hard_constraint_max_sessions(time_table, agent, option, n_sessions,
                                    specs)) {
    return 0;
  }

  if (!hard_constraint_venue_type(time_table, agent, option, n_sessions,
                                  specs)) {
    return 0;
  }

  if (!hard_constraint_sufficient_timeslot(time_table, agent, option,
                                           n_sessions, specs)) {
    return 0;
  }
  // Add other hard constraint ifs

  return 1;
}

void get_feasible_actions(Agent *agent, TimeTableSpecifications *specifications,
                          TimeTableTuple *options, uint n_options,
                          uint *actions, uint *n_actions) {

  *n_actions = 0;

  // We expect options to be an array of size n_options

  // Getting partial time table for this agent state

  // Iterating through all possible options here.
  for (uint i = 0; i < n_options; i++) {
    if (is_feasible(agent->timetable, agent, options + i, agent->depth,
                    specifications)) {
      actions[*n_actions] = i;
      (*n_actions)++;
    }
  }
}
