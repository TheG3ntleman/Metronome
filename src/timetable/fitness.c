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
        findAssociatedParties(i, &number_of_parties, party_id_array, specifications);
        
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
        findAssociatedSessions(i, &number_of_session, session_id_array, specifications);
        
        uint day_array[6];
        for (uint j = 0; j < 6; j++) {
            day_array[j] = 0;
        }
        
        for (uint j = 0; j < number_of_session; j++) {  
            uint venue_id, timeslot_id;
            ttGetTuple(population, timetable_index, session_id_array[j], &venue_id, &timeslot_id); 
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
    
    numeric violations = 0;  // Initialize violations
    
    for (uint i = 0; i < specifications->party_table->size; i++) {
        uint session_id_array[specifications->session_table->size];
        uint number_of_session;
        findAssociatedSessions(i, &number_of_session, session_id_array, specifications);
        uint timeslot_array[specifications->timeslot_table->size];
        uint n = 0;
        for (uint j = 0; j < number_of_session; j++) {
            uint venue_id, timeslot_id;
            ttGetTuple(population, timetable_index, session_id_array[j], &venue_id, &timeslot_id);
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
            for (uint k = i+1; k < duration; k++) {  // Corrected the loop initialization
                uint timeslot_id2, venue_id2;
                ttGetTuple(population, timetable_index, i + k, &venue_id2, &timeslot_id2);  
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
static numeric computeSoftConstraint_studenttraveltime(
    Population *population, TimeTableSpecifications *specifications,
    uint timetable_index) {
  /*Consecutive sessions may need to be scheduled in close proximity to each
   * other to minimize student travel time between them.*/

  return 0;
}
#endif
#ifdef SOFT_MAXIMIZE_CHUNKING
static numeric computeSoftConstraint_maximize_chunking(
    Population *population, TimeTableSpecifications *specifications,
    uint timetable_index) {
  /*maximize session chunking.(have back to back sessions for students)*/

  return 0;
}
#endif
#ifdef SOFT_ROOM_UTILIZATION
static numeric computeSoftConstraint_room_utilization(
    Population *population, TimeTableSpecifications *specifications,
    uint timetable_index) {
    /* Maximize Room Utilization Constraint: Encourage the efficient use of rooms
     by maximizing their overall utilization. */
    numeric violations = 0;

    uint room_utilization[specifications->venue_table->size];
    for (uint i = 0; i < specifications->venue_table->size; i++) {
        room_utilization[i] = 0;
    }

    for (uint i = 0; i < population->n_sessions; i++) {  // Corrected n_sesions to n_sessions
        uint venue_id, timeslot_id;
        ttGetTuple(population, timetable_index, i, &venue_id, &timeslot_id);
        room_utilization[venue_id]++;
    }
    for (uint i = 0; i < specifications->venue_table->size; i++) {  // Corrected venue_table to venue_table->size
        if (room_utilization[i] == 0) {
            violations++;
        }
    }

    return specifications->constraint->weights[8] * violations;
}

#endif
#ifdef SOFT_AVOID_EARLYLATE_TIME
static numeric computeSoftConstraint_avoid_early_late_session(
    Population *population, TimeTableSpecifications *specifications,
    uint timetable_index) {
  /*Avoid Early/Late Times Constraint: Express preferences to avoid scheduling
   * classes too early or too late in the day.*/

  return 0;
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
        uint party_id_array[specifications->party->size];
        uint number_of_parties;
        findAssociatedParties(i, &number_of_parties, party_id_array, specifications);
        uint strength = 0;
        for (uint j = 0; j < number_of_parties; j++) {  // Corrected the loop condition
            strength += specifications->party_table->strength[party_id_array[j]];
        }
        uint capacity = specifications->venue_table->capacity[venue_id];
        violations += capacity - strength;
    }

    return specifications->constraint->weights[10] * violations;
}

#endif
#ifdef SOFT_COMMON_TIMSLOT_EMPTY
static numeric computeSoftConstraint_common_timeslot_empty(
    Population *population, TimeTableSpecifications *specifications,
    uint timetable_index) {
  /*encourage common time slots being left empty for a particular batch,
  so if there are some sessions that were canceled during the week, they can be
  held at that time slot.*/

  return 0;
}
#endif
#ifdef SOFT_MINIMIZE_BACKTOBACK_TEACHERCLASSES
static numeric computeSoftConstraint_backtoback_teacher_class(
    Population *population, TimeTableSpecifications *specifications,
    uint timetable_index) {
  /*Minimization of Back-to-Back Classes for teachers.*/

  return 0;
}
#endif
#ifdef SOFT_MINIMIZE_SAMECOURSE_SESSION
static numeric computeSoftConstraint_samecouse_session(
    Population *population, TimeTableSpecifications *specifications,
    uint timetable_index) {
  /*discourage scheduling sessions of the same course more than once per day.*/

  return 0;
}
#endif
#ifdef SOFT_LAB_AFTER_LECTURE
static numeric computeSoftConstraint_lab_after_lecture(
    Population *population, TimeTableSpecifications *specifications,
    uint timetable_index) {
  /*have lab sessions after the lecture session for a particular course*/

  return 0;
}
#endif
#ifdef SOFT_SESSIONS_EVENLY_THROUGHOUT_WEEK
static numeric computeSoftConstraint_even_distrubution(
    Population *population, TimeTableSpecifications *specifications,
    uint timetable_index) {
  /*have an even distribution of events throughout the week (avoid having too
   * many sessions at one day)*/

  return 0;
}
#endif

static numeric computeHardFitnesses(
    Population *population, TimeTableSpecifications *specifications,
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
  fitness += computeHardConstraint_suffectient_timeslotConstraint(
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
