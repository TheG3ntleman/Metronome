#include "constraint.h"
#include "src/timetable/population.h"
#include "src/timetable/specifications.h"
#include <linux/limits.h>

uint hard_party_conflict_constraint(TimeTableTuple *timetable,
                                    TimeTableSpecifications *specs,
                                    uint depth) {

  uint number_of_violations = 0;

  // Iterating through the parties
  for (uint party_id = 0; party_id < specs->party_table->size; party_id++) {
    uint number_of_sessions = 0;
    uint sessions[specs->session_table->size];
    findAssociatedSessions(party_id, &number_of_sessions, sessions, depth,
                           specs);

    uint timeslot_counts[specs->timeslot_table->size];

    // Initialize the timeslot_counts array to 0
    for (uint i = 0; i < specs->timeslot_table->size; i++)
      timeslot_counts[i] = 0;

    for (uint i = 0; i < number_of_sessions; i++) {
      // To make sure a party is not in two different placese at the
      // same time. We iterate through all the tuples corresponding to
      // the parties associated sessions and see if at any point, the
      // timeslot value of the tuples are the same. This is done
      // by having a counter for each timeslot.

      if (sessions[i] > depth)
        continue;

      timeslot_counts[timetable[sessions[i]].timeslot]++;
    }

    // Accumulate violations here
    for (uint i = 0; i < specs->timeslot_table->size; i++) {
      if (timeslot_counts[i] > 1) {
        number_of_violations += timeslot_counts[i] - 1;
      }
    }
  }

  return number_of_violations;
}

uint hard_repeated_tuple_constraint(TimeTableTuple *timetable,
                                    TimeTableSpecifications *specs,
                                    uint depth) {

  uint number_of_violations = 0;

  uint tuple_counter[specs->timeslot_table->size][specs->venue_table->size];

  // Initialize the tuple_counter array to 0
  for (uint i = 0; i < specs->timeslot_table->size; i++) {
    for (uint j = 0; j < specs->venue_table->size; j++) {
      tuple_counter[i][j] = 0;
    }
  }

  // Iterate through all sessions
  for (uint i = 0; i < depth; i++) {
    tuple_counter[timetable[i].timeslot][timetable[i].venue] += 1;
  }

  // Now we accumulate the violations
  for (uint i = 0; i < specs->timeslot_table->size; i++) {
    for (uint j = 0; j < specs->venue_table->size; j++) {
      if (tuple_counter[i][j] > 1) {
        number_of_violations += tuple_counter[i][j] - 1;
      }
    }
  }

  return number_of_violations;
}

uint hard_venue_capacity_constraint(TimeTableTuple *timetable,
                                    TimeTableSpecifications *specs,
                                    uint depth) {

  uint number_of_violations = 0;
  for (uint session_id = 0; session_id < depth; session_id++) {
    uint number_of_parties = 0;
    uint party_id[specs->party_table->size];
    findAssociatedParties(session_id, &number_of_parties, party_id, depth,
                          specs);

    uint strength = 0;

    for (uint i = 0; i < number_of_parties; i++) {
      strength += specs->party_table->strength[party_id[i]];
    }

    // Checking if the strength exceeds the venue capacity
    if (strength > specs->venue_table->capacity[timetable[session_id].venue]) {
      number_of_violations += 1;
    }
  }

  return number_of_violations;
}

uint hard_venue_type_constraint(TimeTableTuple *timetable,
                                TimeTableSpecifications *specs, uint depth) {
  uint number_of_violations = 0;
  for (uint session_id = 0; session_id < depth; session_id++) {
    if (specs->session_table->type[session_id] !=
        specs->venue_table->type[timetable[session_id].venue]) {
      number_of_violations += 1;
    }
  }

  return number_of_violations;
}

uint hard_max_hours_constraint(TimeTableTuple *timetable,
                               TimeTableSpecifications *specs, uint depth) {

  uint number_of_violations = 0;

  for (uint party_id = 0; party_id < specs->party_table->size; party_id++) {
    uint number_of_sessions = 0;
    uint sessions[specs->session_table->size];
    uint no_of_classes_per_day[5]; // BIG ASSUMPTION CHANGE LATER

    // Initialize the no_of_classes_per_day array to 0
    for (uint i = 0; i < 5; i++)
      no_of_classes_per_day[i] = 0;

    findAssociatedSessions(party_id, &number_of_sessions, sessions, depth,
                           specs);

    for (uint i = 0; i < number_of_sessions; i++) {
      uint timeslot_id = timetable[sessions[i]].timeslot;
      uint day = specs->timeslot_table->day[timeslot_id];
      no_of_classes_per_day[day]++;
    }

    // Checking if the no of sessions per day are exceeded by the threshold
    // amount
    for (uint i = 0; i < 5; i++) {
      if (no_of_classes_per_day[i] > specs->party_table->max_hours[party_id]) {
        number_of_violations +=
            no_of_classes_per_day[i] - specs->party_table->max_hours[party_id];
      }
    }
  }

  return number_of_violations;
}

uint hard_multi_timeslot_constraint(TimeTableTuple *timetable,
                                    TimeTableSpecifications *specs,
                                    uint depth) {

  uint number_of_violations = 0;

  for (uint session_id = 0; session_id < depth; session_id++) {
    
  }

  return number_of_violations;
}