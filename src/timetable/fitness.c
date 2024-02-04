#include "fitness.h"

// Capacity constraint related fitness functions
#ifdef HARD_CAPACITY_CONSTRAINT
static numeric computeHardConstraint_CapacityConstraint(
    Population *population, TimeTableSpecifications *specifications,
    uint timetable_index) {

  numeric violations = 0;
  for (uint i = 0; i < population->n_sessions; i++) {
    uint timeslot_id;
    uint venue_id;
    ttGetTuple(population, timetable_index, i, &venue_id, &timeslot_id);

    uint party_id_array[specifications->party_table->size];
    uint number_of_parties;
    uint strength = 0;
    findAssociatedParties(i, &number_of_parties, party_id_array,
                          specifications);

    for (uint j = 0; j < number_of_parties; j++) {
      strength += specifications->party_table->strength[party_id_array[j]];
    }

    uint capacity = specifications->venue_table->capacity[venue_id];
    if (strength > capacity) {
      violations++;
    }
  }
  return violations;
}
#endif

#ifdef HARD_VENUE_CONFLICT
static numeric computeHardConstraint_VenueConflictConstraint(
    Population *population, TimeTableSpecifications *specifications,
    uint timetable_index) {

  numeric violations = 0;

  // Iterating through sessions
  for (uint i = 0; i < population->n_sessions; i++) {
    uint timeslot1, venue1, timeslot2, venue2;
    ttGetTuple(population, timetable_index, i, &venue1, &timeslot1);
    for (uint j = i + 1; j < population->n_sessions; j++) {
      ttGetTuple(population, timetable_index, j, &venue2, &timeslot2);
      if (timeslot1 == timeslot2 && venue1 == venue2) {
        violations++;
      }
    }
  }

  return violations;
}
#endif

#ifdef HARD_VENUE_TYPE
static numeric computeHardConstraint_VenueTypeConstraint(
    Population *population, TimeTableSpecifications *specifications,
    uint timetable_index) {

  numeric violations = 0;

  // Iterating through sessions
  for (uint i = 0; i < population->n_sessions; i++) {

    uint timeslot, venue;
    ttGetTuple(population, timetable_index, i, &venue, &timeslot);

    // Retrieve required and assigned venue type
    uint venue_type_assigned = specifications->venue_table->type[venue];
    uint venue_type_required = specifications->session_table->type[i];

    // Check if they are matching
    if (venue_type_assigned != venue_type_required) {
      violations++;
    }
  }

  return violations;
}
#endif

#ifdef HARD_MAX_SESSIONS
static numeric computeHardConstraint_MaxSessionsConstraint(
    Population *population, TimeTableSpecifications *specifications,
    uint timetable_index) {

  numeric violations = 0;

  for (uint i = 0; i < specifications->party_table->size; i++) {
    uint session_id_array[specifications->session_table->size];
    uint number_of_session;
    findAssociatedSessions(i, &number_of_session, session_id_array,
                           specifications);

    uint day_array[6];
    for (uint j = 0; j < 6; j++) {
      day_array[j] = 0;
    }

    for (uint j = 0; j < number_of_session; j++) {
      uint venue_id, timeslot_id;
      ttGetTuple(population, timetable_index, session_id_array[j], &venue_id,
                 &timeslot_id);
      uint day = specifications->timeslot_table->day[timeslot_id];
      day_array[day]++;
    }

    for (uint j = 0; j < 6; j++) {
      if (day_array[j] > specifications->party_table->max_hours[i]) {
        violations++;
      }
    }
  }

  return violations;
}
#endif

#ifdef HARD_PARTY_DUPLICATE
static numeric computeHardConstraint_PartyDuplicateConstraint(
    Population *population, TimeTableSpecifications *specifications,
    uint timetable_index) {

  numeric violations = 0; // Initialize violations

  for (uint i = 0; i < specifications->party_table->size; i++) {
    uint session_id_array[specifications->session_table->size];
    uint number_of_session;
    findAssociatedSessions(i, &number_of_session, session_id_array,
                           specifications);
    uint timeslot_array[specifications->timeslot_table->size];
    uint n = 0;
    for (uint j = 0; j < number_of_session; j++) {
      uint venue_id, timeslot_id;
      ttGetTuple(population, timetable_index, session_id_array[j], &venue_id,
                 &timeslot_id);
      timeslot_array[n] = timeslot_id;
      n++;
    }
    for (uint j = 0; j < n; j++) {
      for (uint k = j + 1; k < n; k++) {
        if (timeslot_array[j] == timeslot_array[k]) {
          violations++;
        }
      }
    }
  }

  return violations;
}
#endif

#ifdef HARD_SUFFECIENT_TIMESLOT
static numeric computeHardConstraint_sufficient_timeslotConstraint(
    Population *population, TimeTableSpecifications *specifications,
    uint timetable_index) {

  numeric violations = 0;

  for (uint i = 0; i < population->n_sessions; i++) {
    uint venue_id1, timeslot_id1;
    ttGetTuple(population, timetable_index, i, &venue_id1, &timeslot_id1);
    uint duration = specifications->session_table->duration[i];

    if (duration > 1) {
      for (uint k = i + 1; k < duration;
           k++) { // Corrected the loop initialization
        uint timeslot_id2, venue_id2;
        ttGetTuple(population, timetable_index, i + k, &venue_id2,
                   &timeslot_id2);
        if (venue_id1 == venue_id2) {
          if (timeslot_id1 + k - i != timeslot_id2) {
            violations++;
          }
        } else {
          if (timeslot_id1 + k - i != timeslot_id2) {
            violations += 2;
          } else {
            violations++;
          }
        }
      }
      i = i + duration;
    }
  }

  return violations;
}
#endif

#ifdef SOFT_STUDENT_TRAVELTIME
static numeric
computeSoftConstraint_studenttraveltime(Population *population,
                                        TimeTableSpecifications *specifications,
                                        uint timetable_index) {
  /*Consecutive sessions may need to be scheduled in close proximity to each
   * other to minimize student travel time between them.*/
  numeric violations = 0;

  for (uint i = 0; i < specifications->party_table->size; i++) {
    uint session_id_array[specifications->session_table->size];
    uint number_of_session;
    findAssociatedSessions(i, &number_of_session, session_id_array,
                           specifications);

    uint timeslot_array[specifications->timeslot_table->size];
    uint venue_array[specifications->venue_table->size];
    uint n = 0;
    for (uint j = 0; j < number_of_session; j++) {
      uint venue_id, timeslot_id;
      ttGetTuple(population, timetable_index, session_id_array[j], &venue_id,
                 &timeslot_id);
      timeslot_array[n] = timeslot_id;
      venue_array[n] = venue_id;
      n++;
    }

    for (uint i = 1; i < number_of_session; i++) {
      uint key_timeslot = timeslot_array[i];
      uint key_session_id = session_id_array[i];
      uint key_venue = venue_array[i];
      int j = i - 1;

      while (j >= 0 && timeslot_array[j] > key_timeslot) {
        // Directly swap elements
        timeslot_array[j + 1] = timeslot_array[j];
        session_id_array[j + 1] = session_id_array[j];
        venue_array[j + 1] = venue_array[j];
        j = j - 1;
      }

      // Assign the key values to their correct positions
      timeslot_array[j + 1] = key_timeslot;
      session_id_array[j + 1] = key_session_id;
      venue_array[j + 1] = key_venue;
    }

    for (uint j = 0; j < n - 1; j++) {
      uint day1 = specifications->timeslot_table->day[timeslot_array[j]];
      uint day2 = specifications->timeslot_table->day[timeslot_array[j + 1]];
      if (day1 == day2) {
        uint locality_i = specifications->venue_table->locality[venue_array[j]];
        uint locality_j =
            specifications->venue_table->locality[venue_array[j + 1]];
        uint distance = 0;
        locality_to_distance(locality_i, locality_j, &distance, specifications);
        violations += distance;
      }
    }
  }

  return specifications->constraint_weights[0] * violations;
}
#endif

#ifdef SOFT_MAXIMIZE_CHUNKING
static numeric
computeSoftConstraint_maximize_chunking(Population *population,
                                        TimeTableSpecifications *specifications,
                                        uint timetable_index) {
  /* Maximize session chunking (have back-to-back sessions for students). */
  numeric violations = 0;

  for (uint i = 0; i < specifications->party_table->size; i++) {
    uint session_id_array[specifications->session_table->size];
    uint number_of_session;
    findAssociatedSessions(i, &number_of_session, session_id_array,
                           specifications);
    uint timeslot_array[specifications->timeslot_table->size];
    for (uint j = 0; j < specifications->timeslot_table->size; j++) {
      timeslot_array[j] = 0;
    }
    uint n = 0;
    for (uint j = 0; j < number_of_session; j++) {
      uint venue_id, timeslot_id;
      ttGetTuple(population, timetable_index, session_id_array[j], &venue_id,
                 &timeslot_id);
      timeslot_array[n] = timeslot_id;
      n++;
    }
    for (uint j = 0; j < n - 1;
         j++) { // Loop up to n - 1 to avoid out-of-bounds access
      if (timeslot_array[j] + 1 != timeslot_array[j + 1]) {
        violations++;
      }
    }
  }

  return specifications->constraint_weights[1] * violations;
}
#endif

#ifdef SOFT_ROOM_UTILIZATION
static numeric
computeSoftConstraint_room_utilization(Population *population,
                                       TimeTableSpecifications *specifications,
                                       uint timetable_index) {
  /* Maximize Room Utilization Constraint: Encourage the efficient use of rooms
   by maximizing their overall utilization. */
  numeric violations = 0;

  uint room_utilization[specifications->venue_table->size];
  for (uint i = 0; i < specifications->venue_table->size; i++) {
    room_utilization[i] = 0;
  }

  for (uint i = 0; i < population->n_sessions;
       i++) { // Corrected n_sesions to n_sessions
    uint venue_id, timeslot_id;
    ttGetTuple(population, timetable_index, i, &venue_id, &timeslot_id);
    room_utilization[venue_id]++;
  }
  for (uint i = 0; i < specifications->venue_table->size;
       i++) { // Corrected venue_table to venue_table->size
    if (room_utilization[i] == 0) {
      violations++;
    }
  }

  return specifications->constraint_weights[8] * violations;
}
#endif

#ifdef SOFT_AVOID_EARLYLATE_TIME
static numeric computeSoftConstraint_avoid_early_late_session(
    Population *population, TimeTableSpecifications *specifications,
    uint timetable_index) {
  /* Avoid Early/Late Times Constraint: Express preferences to avoid scheduling
   * classes too early or too late in the day. */
  numeric violations = 0;
  uint number_of_days = specifications->timeslot_table
                            ->day[specifications->timeslot_table->size - 1];
  uint number_of_timeslot_per_day =
      specifications->timeslot_table->size / number_of_days;

  for (uint i = 0; i < specifications->party_table->size; i++) {
    uint session_id_array[specifications->session_table->size];
    uint number_of_session;
    findAssociatedSessions(i, &number_of_session, session_id_array,
                           specifications);
    uint preferred_start_time =
        specifications->party_table->preferred_start_time[i];
    uint preferred_end_time =
        specifications->party_table->preferred_end_time[i];
    for (uint j = 0; j < number_of_session; j++) {
      uint venue_id, timeslot_id;
      ttGetTuple(population, timetable_index, session_id_array[j], &venue_id,
                 &timeslot_id);
      if ((timeslot_id / number_of_timeslot_per_day) < preferred_start_time ||
          (timeslot_id / number_of_timeslot_per_day) > preferred_end_time) {
        violations++;
      }
    }
  }

  return specifications->constraint_weights[9] * violations;
}
#endif

#ifdef SOFT_ROOM_CAPACITY_UTILIZATION
static numeric computeSoftConstraint_room_capacity_utilization(
    Population *population, TimeTableSpecifications *specifications,
    uint timetable_index) {
  /* Avoid scheduling sessions with less strength to venues which have high
    capacity ex: don't schedule ECM-only class at auditorium */
  numeric violations = 0;

  for (uint i = 0; i < population->n_sessions; i++) {
    uint venue_id, timeslot_id;
    ttGetTuple(population, timetable_index, i, &venue_id, &timeslot_id);
    uint party_id_array[specifications->party_table->size];
    uint number_of_parties;
    findAssociatedParties(i, &number_of_parties, party_id_array,
                          specifications);
    uint strength = 0;
    for (uint j = 0; j < number_of_parties;
         j++) { // Corrected the loop condition
      strength += specifications->party_table->strength[party_id_array[j]];
    }
    uint capacity = specifications->venue_table->capacity[venue_id];
    violations += capacity - strength;
  }

  return specifications->constraint_weights[10] * violations;
}
#endif

#ifdef SOFT_COMMON_TIMSLOT_EMPTY
static numeric computeSoftConstraint_common_timeslot_empty(
    Population *population, TimeTableSpecifications *specifications,
    uint timetable_index) {
  /* Encourage common time slots being left empty for a particular batch,
  so if there are some sessions that were canceled during the week, they can be
  held at that time slot. */
  numeric violations = 0;

  uint timeslot_array[specifications->timeslot_table->size];
  for (uint i = 0; i < specifications->timeslot_table->size; i++) {
    timeslot_array[i] = 0;
  }

  for (uint i = 0; i < specifications->session_table->size; i++) {
    uint venue_id, timeslot_id;
    ttGetTuple(population, timetable_index, i, &venue_id, &timeslot_id);
    timeslot_array[timeslot_id]++;
  }
  for (uint i = 0; i < specifications->timeslot_table->size; i++) {
    if (timeslot_array[i] != 0) {
      violations++;
    }
  }

  return specifications->constraint_weights[11] * violations;
}
#endif

#ifdef SOFT_MINIMIZE_BACKTOBACK_TEACHERCLASSES
static numeric computeSoftConstraint_backtoback_teacher_class(
    Population *population, TimeTableSpecifications *specifications,
    uint timetable_index) {
  /* Minimization of Back-to-Back Classes for teachers. */

  numeric violations = 0;
  uint number_of_days = specifications->timeslot_table
                            ->day[specifications->timeslot_table->size - 1];
  uint number_of_timeslot_per_day =
      specifications->timeslot_table->size / number_of_days;

  for (uint i = 0; i < specifications->party_table->size; i++) {

    if (specifications->party_table->party_type[i] == 1) {

      uint session_id_array[specifications->session_table->size];
      uint number_of_session;
      findAssociatedSessions(i, &number_of_session, session_id_array,
                             specifications);

      for (uint j = 0; j < number_of_session - 1; j = j + 2) {

        uint venue_id_prev, timeslot_id_prev;
        ttGetTuple(population, timetable_index, session_id_array[j],
                   &venue_id_prev, &timeslot_id_prev);
        uint venue_id_next, timeslot_id_next;
        ttGetTuple(population, timetable_index, session_id_array[j + 1],
                   &venue_id_next, &timeslot_id_next);

        if ((timeslot_id_prev / number_of_timeslot_per_day) + 1 ==
            (timeslot_id_next / number_of_timeslot_per_day)) {
          violations++;
        }
      }
    }
  }

  return specifications->constraint_weights[12] * violations;
}
#endif

#ifdef SOFT_MINIMIZE_SAMECOURSE_SESSION
static numeric computeSoftConstraint_samecourse_session(
    Population *population, TimeTableSpecifications *specifications,
    uint timetable_index) {
  /* Discourage scheduling sessions of the same course more than once per day.
   */
  numeric violations = 0;

  for (uint i = 0; i < specifications->session_table->size; i++) {

    uint venue_id1, timeslot_id1;
    ttGetTuple(population, timetable_index, i, &venue_id1, &timeslot_id1);
    uint day1 = specifications->timeslot_table->day[timeslot_id1];

    for (uint j = i + 1; j < specifications->session_table->size; j++) {

      if (specifications->session_table->course[i] ==
          specifications->session_table->course[j]) {

        uint venue_id2, timeslot_id2;
        ttGetTuple(population, timetable_index, j, &venue_id2, &timeslot_id2);
        uint day2 = specifications->timeslot_table->day[timeslot_id2];

        if (day1 == day2) {
          violations++;
        }
      }
    }
  }

  return specifications->constraint_weights[13] * violations;
}
#endif

#ifdef SOFT_LAB_AFTER_LECTURE
static numeric
computeSoftConstraint_lab_after_lecture(Population *population,
                                        TimeTableSpecifications *specifications,
                                        uint timetable_index) {
  /*have lab sessions after the lecture session for a particular course*/

  return 0;
}
#endif

#ifdef SOFT_SESSIONS_EVENLY_THROUGHOUT_WEEK
static numeric
computeSoftConstraint_even_distribution(Population *population,
                                        TimeTableSpecifications *specifications,
                                        uint timetable_index) {
  /* Have an even distribution of events throughout the week (avoid having too
   * many sessions on one day). */
  numeric violations = 0;
  uint number_of_days = specifications->timeslot_table
                            ->day[specifications->timeslot_table->size - 1];

  for (uint i = 0; i < specifications->party_table->size; i++) {
    uint session_id_array[specifications->session_table->size];
    uint number_of_session;
    findAssociatedSessions(i, &number_of_session, session_id_array,
                           specifications);

    uint day_array[number_of_days];
    for (uint j = 0; j < number_of_days; j++) {
      day_array[j] = 0;
    }

    for (uint j = 0; j < number_of_session; j++) {
      uint venue_id, timeslot_id;
      ttGetTuple(population, timetable_index, session_id_array[j], &venue_id,
                 &timeslot_id);
      uint day = specifications->timeslot_table
                     ->day[timeslot_id]; // Corrected day access
      day_array[day]++;
    }

    uint sum = 0;
    for (uint j = 0; j < number_of_days; j++) {
      sum += day_array[j];
    }

    uint average = sum / number_of_days;
    for (uint j = 0; j < number_of_days; j++) {
      if (day_array[j] >= average) {
        violations += day_array[j] - average;
      } else {
        violations += average - day_array[j];
      }
    }
  }

  return specifications->constraint_weights[15] * violations;
}
#endif

static numeric computeHardFitnesses(Population *population,
                                    TimeTableSpecifications *specifications,
                                    uint timetable_index) {

  numeric fitness = 0;

#ifdef HARD_CAPACITY_CONSTRAINT
  fitness += computeHardConstraint_CapacityConstraint(
      population, specifications, timetable_index);
#endif

#ifdef HARD_VENUE_CONFLICT
  fitness += computeHardConstraint_VenueConflictConstraint(
      population, specifications, timetable_index);
#endif

#ifdef HARD_VENUE_TYPE
  fitness += computeHardConstraint_VenueTypeConstraint(
      population, specifications, timetable_index);
#endif

#ifdef HARD_MAX_SESSIONS
  // This function has a problem.
  fitness += computeHardConstraint_MaxSessionsConstraint(
      population, specifications, timetable_index);
#endif

#ifdef HARD_PARTY_DUPLICATE
  fitness += computeHardConstraint_PartyDuplicateConstraint(
      population, specifications, timetable_index);
#endif

#ifdef HARD_SUFFECIENT_TIMESLOT
  fitness += computeHardConstraint_sufficient_timeslotConstraint(
      population, specifications, timetable_index);
#endif

  return fitness;
}

void computeFitnesses(Population *population,
                      TimeTableSpecifications *specfications,
                      numeric *soft_fitness, numeric *hard_fitness) {

  // Iterate through all members of the population

  for (uint i = 0; i < population->n_timetables; i++) {
    soft_fitness[i] = 0;
    hard_fitness[i] = 500 - computeHardFitnesses(population, specfications, i);
  }
}
